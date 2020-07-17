#include "RTreeAOI.h"
#include "Util/Util.h"
#include "Pool/ObjectPool.h"
#include "Log/Logger.h"

void TRTreeAOI::enter(int64_t id, Vector3D &pos)
{
    VectorNode *node = NEW_OBJECT(VectorNode, id, pos, _boundVector);
    _objTree.Insert(node->_pos.x, node->_posR.x, node);
    _objMap.insert(std::make_pair(id, node));
}
void TRTreeAOI::leave(int64_t id)
{
    auto it = _objMap.find(id);
    if (it != _objMap.end())
    {
        _objTree.Remove(it->second->_pos.x, it->second->_posR.x, it->second);
        DELETE_OBJECT(it->second);
        _objMap.erase(it);
    }
}
void TRTreeAOI::update(int64_t id, Vector3D &pos)
{
    auto it = _objMap.find(id);
    if (it != _objMap.end())
    {
        _objTree.Remove(it->second->_pos.x, it->second->_posR.x, it->second);
        it->second->Update(pos, _boundVector);
        _objTree.Insert(it->second->_pos.x, it->second->_posR.x, it->second);
    }
}
bool TRTreeAOI::callback(VectorNode *const &data)
{
    _context->_now.push_back(data->_id);
    if (_context->_now.size() > MAX_CHECK_NUM)
    {
        return false;
    }
    return true;
}
void TRTreeAOI::loop()
{
    for (auto &it : _objMap)
    {
        auto *obj = it.second;
        _context = obj;
        obj->_last.clear();
        obj->_add.clear();
        obj->_rm.clear();

        obj->_last.swap(obj->_now);
        _objTree.Search(obj->_left.x, obj->_right.x, std::bind(&TRTreeAOI::callback, this, std::placeholders::_1));
        obj->_now.sort();
        Difference(obj->_last.begin(), obj->_last.end(), obj->_now.begin(), obj->_now.end(),
                   std::insert_iterator<TList>(obj->_rm, obj->_rm.begin()), std::insert_iterator<TList>(obj->_add, obj->_add.begin()));
    }
}
void TRTreeAOI::clear()
{
    _objTree.RemoveAll();
    for (auto it = _objMap.begin(); it != _objMap.end(); ++it)
    {
        DELETE_OBJECT(it->second);
    }
    _objMap.clear();
}

void TRTreeAOI::echoInterest(int64_t id)
{
    auto iter = _objMap.find(id);
    VectorNode *obj = nullptr;
    int32_t mark = 0;
    if (iter != _objMap.end())
    {
        obj = iter->second;
        LOG_INFO("Obj " << id << ":");
        LOG_INFO("Last Interest ID:");
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        for (auto tmp = obj->_last.begin(); tmp != obj->_last.end(); tmp++)
        {
            snprintf(buf + 20 * mark, 20, "     %ld", *tmp);
            if (mark > FORMAT_NUM)
            {
                snprintf(buf + 20 * mark, 20, "\n");
                mark = 0;
            }
            mark++;
        }
        LOG_INFO(buf);
        memset(buf, 0, sizeof(buf));
        mark = 0;
        LOG_INFO(" Now Interest ID:");
        for (auto tmp = obj->_now.begin(); tmp != obj->_now.end(); tmp++)
        {
            snprintf(buf + 20 * mark, 20, "%-10ld", *tmp);
            if (mark > FORMAT_NUM)
            {
                snprintf(buf + 20 * mark, 20, "\n");
                mark = 0;
            }
            mark++;
        }
        LOG_INFO(buf);
        LOG_INFO("Size = "<< obj->_now.size());
    }
}
