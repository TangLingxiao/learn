#pragma once
#include "Logic/AOI/Vector.h"
#include "Structure/RTreeTemplate/RTree.h"
#include <list>
#include <unordered_map>

class TRTreeAOI
{
public:
    enum
    {
        FORMAT_NUM = 5,
        MAX_CHECK_NUM = 128,
    };
    struct VectorNode;
    using TList = std::list<int64_t>;
    using ObjMap = std::unordered_map<int64_t, VectorNode *>;
    using ObjTree = RTree<VectorNode *, int32_t, 3, int32_t, 64, 32>;

    struct VectorNode
    {
        int8_t _type;
        int64_t _id;
        TList *_members;
        Vector3D _pos;
        Vector3D _posR;
        Vector3D _left;
        Vector3D _right;
        TList _last;
        TList _now;
        TList _add;
        TList _rm;

        VectorNode(int64_t id, Vector3D &pos, Vector3D &boundary) : _id(id), _pos(pos), _posR(pos), _left(pos), _right(pos)
        {
            Vector3D one(1);
            _left - boundary;
            _right + boundary;
            _posR + one;
        }

        void Update(Vector3D &pos, Vector3D &boundary)
        {
            Vector3D one(1);
            _pos = pos;
            _posR = pos;
            _left = pos;
            _right = pos;
            _left - boundary;
            _right + boundary;
            _posR + one;
        }
    };

public:
    void enter(int64_t id, Vector3D &pos);
    void leave(int64_t id);
    void update(int64_t id, Vector3D &pos);
    bool callback(VectorNode *const &data);
    void loop();
    void clear();

    void setBoundary(Vector3D &boundary) { _boundVector = boundary; }
    void echoInterest(int64_t);

private:
    ObjMap _objMap;
    ObjTree _objTree;
    Vector3D _boundVector;
    VectorNode *_context{nullptr};
};