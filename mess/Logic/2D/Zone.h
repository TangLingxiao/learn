#pragma once
#include <unordered_set>
#include <functional>

using GridID_T = int32_t;
struct WORLD_POS2D
{
    int32_t x;
    int32_t z;
    WORLD_POS2D(int32_t px = 0, int32_t pz = 0) : x(px), z(pz) {}
};

class Zone
{
    struct Rect
    {
        int32_t startX{0};
        int32_t startZ{0};
        int32_t endX{0};
        int32_t endZ{0};
        bool isContain(int32_t x, int32_t z) const
        {
            return !(x < startX || x > endX || z < startZ || z > endZ);
        }
    };

public:
    using ElementCallBackFun = std::function<bool(const int64_t &data)>;
    enum
    {
        INVALID_ID = -1,
    };

private:
    class Grid
    {
    public:
        Grid(){}
        ~Grid() { _data.clear(); }
        void enterGrid(const int64_t &data)
        {
            _data.insert(data);
        }
        void leaveGrid(const int64_t &data)
        {
            _data.erase(data);
        }
        bool sweepAll(const ElementCallBackFun &cb)
        {
            for (auto data : _data)
            {
                if (!cb(data))
                    return false;
            }
            return true;
        }

    private:
        std::unordered_set<int64_t> _data;
    };

public:
    ~Zone();
    bool init(const WORLD_POS2D &leftUpPos, const WORLD_POS2D &rightDownPos, int32_t gridLen);
    void clean();
    GridID_T calZoneID(WORLD_POS2D pos);
    int32_t calGridSize(int32_t dis);
    bool enterZone(int64_t id, GridID_T gridId);
    bool leaveZone(int64_t id, GridID_T gridId);
    void getAllIDsByRadius(GridID_T gridId, int32_t gridRadius, const ElementCallBackFun &cb);
    void getIDsByRadiusFromInner(GridID_T gridId, int32_t gridRadius, const ElementCallBackFun &cb);
    void getAsubBIds(GridID_T gridA, GridID_T gridB, int32_t radius, const ElementCallBackFun &cb);
    void getAIntersectionBIds(GridID_T gridA, GridID_T gridB, int32_t radius, const ElementCallBackFun &cb);

private:
    bool checkGridID(GridID_T gridId);
    bool checkX(int32_t x);
    bool checkZ(int32_t z);
    void getRectInRadius(Rect *rect, GridID_T gridId, int32_t radius);

private:
    WORLD_POS2D _leftUpPos;
    int32_t _w;
    int32_t _h;
    int64_t _size;
    int32_t _gridLen;
    Grid *_grids;
};