#pragma once
#include <deque>
#include <mutex>
#include <atomic>
#include "RedisUtil.h"
#include "Reply.h"
#include "Pattern/Singleton.h"
#include "Async/AsyncQueue.h"
#include "Structure/LockFreeList.h"
#include "AsyncCB.h"
#include <unistd.h>
#include "3rdparty/include/libco/co_routine.h"

#define ASYNC_POOL_SIZE (1)
namespace hiredis
{
    enum RedisResult
    {
        RedisResult_NoData = -999980,
    };

    enum SetCmdOption
    {
        SetCmdOption_None,
        SetCmdOption_XX,
        SetCmdOption_NX,
    };

    enum RangeOption
    {
        RangeOption_BothClose = 0,
        RangeOption_LeftOpen = 1,
        RangeOption_RightOpen = 2,
    };

    struct ClientOptions
    {
        std::string m_sHost;
        int m_iPort{6379};
        std::string m_sPasswd;
        long m_iTimeoutMS{3000};
        int m_iRetryTimes{10};
        void set(const std::map<std::string, std::string> &m);
    };

    typedef std::vector<std::string> StringVector;
    typedef std::map<std::string, std::string> StringMap;

    struct CmdArgNode
    {
        CmdArg cmd;
        RedisAsyncCbPtr m_cb;
        RedisAsyncCallType type;
        AtomicIntrusiveLinkedListHook<CmdArgNode> hook;
    };
    using CmdList = AtomicIntrusiveLinkedList<CmdArgNode, &CmdArgNode::hook>;
    class Client : public std::enable_shared_from_this<Client>
    {
    public:
        Client() {}
        ~Client()
        {
        }
        bool init(const ClientOptions &options);
        bool reconnect();
        bool disconn();
        bool checkconn();
        ReplyPtr command(CmdArg &vdatas);

        int auth(const std::string &pwd);
        int keys(const std::string &key, std::vector<std::string> &vals);
        int delString(const std::string &key);
        int delString(const std::vector<std::string> &keys);
        int ttl(const std::string &key);
        int quit();
        bool ping();
#include "SyncImpl.h"
#include "AsyncImpl.h"
        //pipeline
        CmdList m_CmdList;
        bool m_bPostTask{false};
        std::unique_ptr<IAsyncHandler> m_asyncHandler{nullptr};
        void addCommand(CmdArg &vdatas);
        std::vector<ReplyPtr> execCommand(int nums);

    private:
    private:
        redisContext *m_ctx{nullptr};
        ClientOptions _options;
    };

    using HiredisPrx = std::shared_ptr<Client>;
    using RedisProxy = Client;
    class Pipeline
    {
    public:
        Pipeline(HiredisPrx &client) : _redis(client) {}

        template <typename Arg, typename... Args>
        void addCommand(const Arg &cmd, const Args &... argv)
        {
            CmdArg cmds;
            cmds.append(cmd, argv...);
            _vCmds.push_back(cmds);
            _redis->addCommand(cmds);
        }

        void addCommand(CmdArg &cmd)
        {
            _vCmds.push_back(cmd);
            _redis->addCommand(cmd);
        }
        std::vector<ReplyPtr> exec()
        {
            auto replys =  _redis->execCommand(static_cast<int>(_vCmds.size()));
            while(replys.size() != _vCmds.size())
            {
                //reconnect
                if(!_redis->reconnect())
                {
                    usleep(1000 * 1000);
                    continue; 
                }
                for(size_t i = replys.size(); i < _vCmds.size(); ++i)
                {
                    _redis->addCommand(_vCmds[i]);
                }
                auto replys2 = _redis->execCommand(static_cast<int>(_vCmds.size() - replys.size()));
                for(auto & reply : replys2)
                {
                    replys.emplace_back(std::move(reply));
                }
            }
            return replys;
        }

    private:
        HiredisPrx &_redis;
        std::vector<CmdArg> _vCmds;
    };
    
    class RedisAsyncHandler : public IAsyncHandler
    {
    public:
        RedisAsyncHandler(const HiredisPrx &client) : m_client(client)
        {
        }
        virtual bool onExecute(IKernel *kernel, int32_t queueId, int32_t threadid)
        {
            Pipeline pipeline(m_client);
            std::vector<std::pair<RedisAsyncCallType, RedisAsyncCbPtr>> cbs;
            m_client->m_CmdList.SweepOnce([this, &pipeline, &cbs](CmdArgNode *node) {
                pipeline.addCommand(node->cmd);
                cbs.emplace_back(std::make_pair(node->type, std::move(node->m_cb)));
                delete node;
            });
            auto replys = pipeline.exec();
            if(replys.size() != cbs.size())
            {
                throw std::runtime_error("pipeline error");
            }
            for(size_t i = 0; i < replys.size(); ++i)
            {
                cbs[i].second->onCall(cbs[i].first ,replys[i]);
            }
            m_client->m_bPostTask = false;
            return true;
        }
        virtual bool onSuccess(IKernel *kernel)
        {
            return true;
        }
        virtual bool onFailed(IKernel *kernel)
        {
            return true;
        }
        virtual bool onRelease(IKernel *kernel)
        {
            return true;
        }

    public:
        HiredisPrx m_client;
    };

    class ClientMgr : public Singleton<ClientMgr>
    {
    public:
        ClientMgr();
        ~ClientMgr();
        HiredisPrx getRedisClient();
        void releaseClient(HiredisPrx client);
        void init(const ClientOptions &stClientConf);
        void loop();
        template <RedisAsyncCallType type>
        void startAsync(const RedisAsyncCbPtr &cb, const char *file, const int32_t line, const CmdArg &cmds)
        {
            uint64_t index = m_index.fetch_add(1);
            HiredisPrx client = m_asyncPool[index % ASYNC_POOL_SIZE];
            CmdArgNode *node = new CmdArgNode;
            node->cmd = cmds;
            node->m_cb = cb;
            node->type = type;
            client->m_CmdList.InsertHead(node);
            if (!client->m_bPostTask)
            {
                m_asyncQueue.startAsync(index % ASYNC_POOL_SIZE, client->m_asyncHandler.get(), file, line);
                client->m_bPostTask = true;
            }
        }

    private:
        std::mutex m_mtx;
        std::deque<HiredisPrx> m_Pool;
        ClientOptions m_options;
        std::atomic<uint64_t> m_index{0};
        std::vector<HiredisPrx> m_asyncPool;
        AsyncQueue m_asyncQueue;
    };
#define CLIENT_MGR ClientMgr::getInstance()

    //用于redis统计
    class RedisStatMgr : public Singleton<RedisStatMgr>
    {
    public:
        void addRedisStat(CmdArg &stCmdArg);

    private:
        std::mutex m_mutex;
        std::map<std::string, std::map<std::string, int64_t>> m_mStatCmds;
        int64_t m_iLastStatTime{0};
    };

    
} // namespace hiredis
