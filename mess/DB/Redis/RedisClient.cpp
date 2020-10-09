#include "RedisClient.h"
#include <cstring>
#include <cassert>
#include "Reply.h"
#include "Util/TimeHelper.h"
#include "Log/Logger.h"
#include "Async/AsyncQueue.h"
#include <unistd.h>
#include "libgo.h"

using namespace hiredis;

void ClientOptions::set(const std::map<std::string, std::string> &m)
{
    std::map<std::string, std::string>::const_iterator it;
    if ((it = m.find("host")) != m.end())
        m_sHost = it->second;
    if ((it = m.find("port")) != m.end())
        m_iPort = atoi(it->second.c_str());
    if ((it = m.find("password")) != m.end())
        m_sPasswd = it->second;
    if ((it = m.find("timeout")) != m.end())
        m_iTimeoutMS = atol(it->second.c_str());
    if ((it = m.find("retry")) != m.end())
        m_iRetryTimes = atoi(it->second.c_str());
}

bool Client::init(const ClientOptions &options)
{
    _options = options;
    long &iTimeoutMS = _options.m_iTimeoutMS;
    if (iTimeoutMS < 1000)
    {
        iTimeoutMS = 1000; //最小值保护
    }
    timeval tv;
    tv.tv_sec = iTimeoutMS / 1000;
    tv.tv_usec = (iTimeoutMS % 1000) * 1000;

    m_ctx = redisConnectWithTimeout(options.m_sHost.c_str(), options.m_iPort, tv);
    if (m_ctx && m_ctx->err)
    {
        LOG_ERROR("Connect redis error: " << m_ctx->errstr);
        redisFree(m_ctx);
        m_ctx = nullptr;
        return false;
    }
    else if (m_ctx == nullptr)
    {
        LOG_ERROR("Connect redis error");
        return false;
    }
    if (!options.m_sPasswd.empty())
    {
        auth(options.m_sPasswd);
    }
    if (!ping())
    {
        LOG_ERROR("Connect redis error");
        redisFree(m_ctx);
        return false;
    }
    LOG_DEBUG("Connect redis success, " << options.m_sHost << ":" << options.m_iPort);
    //
    m_asyncHandler.reset(new RedisAsyncHandler(shared_from_this()));
    
    return true;
}

bool Client::reconnect()
{
    disconn();
    return init(_options);
}

bool Client::disconn()
{
    if (m_ctx)
    {
        redisFree(m_ctx);
        m_ctx = nullptr;
    }
    return true;
}

bool Client::checkconn()
{
    return m_ctx ? true : false;
}

ReplyPtr Client::command(CmdArg &vdatas)
{
    auto reply = (redisReply *)redisCommandArgv(m_ctx, vdatas.size(), vdatas.argv(), vdatas.argv_len());
    RedisStatMgr::getInstance().addRedisStat(vdatas);
    int32_t retry{0};
    while (!reply && (!m_ctx || m_ctx->err == REDIS_ERR_EOF || m_ctx->err == REDIS_ERR_IO) && retry++ < _options.m_iRetryTimes)
    {
        if (!reconnect())
        {
            usleep(1000 * 1000);
        }
        else
        {
            reply = (redisReply *)redisCommandArgv(m_ctx, vdatas.size(), vdatas.argv(), vdatas.argv_len());
        }
    }
    return ReplyPtr{reply};
}

void Client::addCommand(CmdArg &vdatas)
{
    if (redisAppendCommandArgv(m_ctx, vdatas.size(), vdatas.argv(), vdatas.argv_len()) != REDIS_OK)
    {
        throw std::runtime_error("append command error");
    }
}

std::vector<ReplyPtr> Client::execCommand(int nums)
{
    std::vector<ReplyPtr> replys;
    void *reply = nullptr;
    while (nums--)
    {
        auto ret = redisGetReply(m_ctx, &reply);
        if(ret != REDIS_OK)
        {
            break;
        }
        replys.emplace_back(ReplyPtr{(redisReply *)reply});
    }
    return replys;
}

int Client::auth(const std::string &pwd)
{
    CmdArg argv;
    argv << "auth" << pwd;
    auto reply = command(argv);
    parser<void>(reply);
    return 0;
}

int Client::keys(const std::string &key, std::vector<std::string> &vals)
{
    CmdArg argv;
    argv << "keys" << key;
    auto reply = command(argv);
    vals = parser<std::vector<std::string>>(reply);
    return 0;
}

int Client::ttl(const std::string &key)
{
    CmdArg argv;
    argv << "ttl" << key;
    auto reply = command(argv);
    return parser<long long>(reply);
}

int Client::quit()
{
    CmdArg argv;
    argv << "quit";
    auto reply = command(argv);
    parser<void>(reply);
    return 0;
}

bool Client::ping()
{
    CmdArg argv;
    argv << "ping";
    auto reply = command(argv);
    if (!reply)
    {
        return false;
    }
    auto res = to_status(*reply);
    return strcasecmp(res.data(), "PONG") == 0;
}

HiredisPrx ClientMgr::getRedisClient()
{
    HiredisPrx redisClient;
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        if (m_Pool.empty())
        {
            redisClient.reset(new Client(), [](Client *p) { p->disconn(); });
        }
        else
        {
            redisClient.swap(m_Pool.front());
            m_Pool.pop_front();
        }
    }
    if (!redisClient->checkconn())
    {
        redisClient->init(m_options);
    }
    return redisClient;
}

void ClientMgr::releaseClient(HiredisPrx client)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    m_Pool.push_back(client);
}
ClientMgr::ClientMgr() : m_asyncQueue(0, "redisClientMgr")
{
}
ClientMgr::~ClientMgr()
{
    for (const auto &client : m_asyncPool)
    {
        client->disconn();
    }
    m_asyncPool.clear();
}

void ClientMgr::init(const ClientOptions &stClientConf)
{
    m_options = stClientConf;
    for (int32_t i = 0; i < ASYNC_POOL_SIZE; ++i)
    {
        auto client = getRedisClient();
        m_asyncPool.emplace_back(std::move(client));
    }
    m_asyncQueue.init(ASYNC_POOL_SIZE);
    // std::thread t([]{ co_sched.Start(); });
    // t.detach();
}

void ClientMgr::loop()
{
    m_asyncQueue.loop();
}

#define REDIS_STAT_INTERVAL 300 //redis统计间隔
void RedisStatMgr::addRedisStat(CmdArg &stCmdArg)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    int64_t iNow = UtilTime::getNow();
    int64_t iNewPoint = iNow / REDIS_STAT_INTERVAL;
    int64_t iLastPoint = m_iLastStatTime / REDIS_STAT_INTERVAL;
    if (iNewPoint > iLastPoint) //每5分钟一个节点，每个节点输出一次
    {
        m_iLastStatTime = iNow;

        for (auto it = m_mStatCmds.cbegin(); it != m_mStatCmds.cend(); ++it)
        {
            const auto &mKeys = it->second;
            for (auto itKey = mKeys.cbegin(); itKey != mKeys.cend(); ++itKey)
            {
                DAYLOG("redisstat", iLastPoint << "|" << it->first << "|" << itKey->first << "|" << itKey->second);
            }
        }
        m_mStatCmds.clear();
    }

    string sCmd = "unknown_cmd";
    string sKey = "unknown_key";
    if (stCmdArg.size() > 0)
    {
        sCmd = std::string(stCmdArg.argv()[0], stCmdArg.argv_len()[0]);
        if (stCmdArg.size() > 1) //既要统计redis命令，又要统计命令的参数(如果有参数的话)
        {
            sKey = std::string(stCmdArg.argv()[1], stCmdArg.argv_len()[1]);
        }
    }
    auto it = m_mStatCmds.find(sCmd);
    if (it != m_mStatCmds.end())
    {
        ++it->second[sKey];
    }
    else
    {
        map<string, int64_t> m;
        m[sKey] = 1;
        m_mStatCmds[sCmd] = m;
    }
}
