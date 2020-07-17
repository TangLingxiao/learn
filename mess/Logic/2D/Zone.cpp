#include "Zone.h"
#include "Util/Util.h"
#include "Log/Logger.h"
Zone::~Zone()
{
    clean();
}

bool Zone::init(const WORLD_POS2D &leftUpPos, const WORLD_POS2D &rightDownPos, int32_t gridLen)
{
    if (!(leftUpPos.x < rightDownPos.x && leftUpPos.z < rightDownPos.z))
    {
        LIB_ASSERT(false, "x error");
        LIB_ASSERT(false, "z error");
        return false;
    }
    _leftUpPos = leftUpPos;
    _gridLen = gridLen;
    int32_t lenX = rightDownPos.x - leftUpPos.x;
    int32_t lenZ = leftUpPos.z - rightDownPos.z;
    _w = lenX / gridLen;
    _h = lenZ / gridLen;
    if (lenX % gridLen > 0)
        ++_w;
    if (lenZ % gridLen > 0)
        ++_h;
    _size = _w * _h;
    LOG_INFO("####################w=" << _w << ",h=" << _h << "################");
    LIB_ASSERT(_size <= 0XFFFF, "size = %ld", _size);
    _grids = new Grid[_size];
    LIB_ASSERT(_grids, "_grids is null");
    return true;
}

void Zone::clean()
{
    if (_grids)
    {
        delete[] _grids;
        _grids = nullptr;
    }
}

GridID_T Zone::calZoneID(WORLD_POS2D pos)
{
    if (pos.x < _leftUpPos.x || pos.z < _leftUpPos.z)
    {
        return INVALID_ID;
    }
    pos.x -= _leftUpPos.x;
    pos.z -= _leftUpPos.z;
    int32_t nX = pos.x / _gridLen;
    int32_t nZ = pos.z / _gridLen;

    GridID_T ret = (nX + nZ * _w);
    if (ret >= _size)
        return INVALID_ID;
    return ret;
}

int32_t Zone::calGridSize(int32_t dis)
{
    return dis / _gridLen + 1;
}

bool Zone::enterZone(int64_t id, GridID_T gridId)
{
    if (checkGridID(gridId))
    {
        (_grids + gridId)->enterGrid(id);
        return true;
    }
    return false;
}

bool Zone::leaveZone(int64_t id, GridID_T gridId)
{
    if (checkGridID(gridId))
    {
        (_grids + gridId)->leaveGrid(id);
        return true;
    }
    return false;
}

void Zone::getAllIDsByRadius(GridID_T gridId, int32_t gridRadius, const ElementCallBackFun &cb)
{
    if (cb == nullptr)
    {
        LOG_ERROR("cb is null");
        return;
    }
    if (!checkGridID(gridId))
    {
        LOG_ERROR("gridId error, gridId:" << gridId);
        return;
    }
    int32_t x = gridId % _w;
    int32_t z = gridId / _w;
    int32_t startX = 0, startZ = 0, endX = 0, endZ = 0;
    startX = (x - gridRadius > 0) ? (x - gridRadius) : 0;
    startZ = (z - gridRadius > 0) ? (z - gridRadius) : 0;
    endX = (x + gridRadius >= _w) ? _w - 1 : x + gridRadius;
    endZ = (z + gridRadius >= _h) ? _h - 1 : z + gridRadius;
    for (z = startZ; z <= endZ; ++z)
    {
        for (x = startX; x <= endX; ++x)
        {
            GridID_T temp = x + z * _w;
            LIB_ASSERT(temp >= 0 && temp < _size, "temp error");
            if (!(_grids + temp)->sweepAll(cb))
                return;
        }
    }
}

void Zone::getIDsByRadiusFromInner(GridID_T gridId, int32_t gridRadius, const ElementCallBackFun &cb)
{
    if (cb == nullptr)
    {
        LOG_ERROR("cb is null");
        return;
    }
    if (!checkGridID(gridId))
    {
        LOG_ERROR("gridId error, gridId:" << gridId);
        return;
    }
    // int32_t x = gridId % _w;
    // int32_t z = gridId / _w;
    // int32_t startX = 0, startZ = 0, endX = 0, endZ = 0;
    // for (int32_t i = 0; i <= gridRadius; ++i)
    // {
    //     startX = x - i;
    //     startZ = z - i;
    //     endX = x + i;
    //     endZ = z + i;
    // }
}

void Zone::getAsubBIds(GridID_T gridA, GridID_T gridB, int32_t radius, const ElementCallBackFun &cb)
{
    if (cb == nullptr)
    {
        LOG_ERROR("cb is null");
        return;
    }
    if (!checkGridID(gridA) || !checkGridID(gridB))
    {
        LOG_ERROR("gridId error, gridId:" << gridA);
        return;
    }
    Rect rectA, rectB;
    getRectInRadius(&rectA, gridA, radius);
    getRectInRadius(&rectB, gridB, radius);
    for (int32_t nz = rectA.startZ; nz <= rectA.endZ; ++nz)
    {
        for (int32_t nx = rectA.startX; nx <= rectA.endX; ++nx)
        {
            if (rectB.isContain(nx, nz))
            {
                continue;
            }
            GridID_T id = nx + nz * _w;
            if (!(_grids + id)->sweepAll(cb))
                return;
        }
    }
}

void Zone::getAIntersectionBIds(GridID_T gridA, GridID_T gridB, int32_t radius, const ElementCallBackFun &cb)
{
    if (cb == nullptr)
    {
        LOG_ERROR("cb is null");
        return;
    }
    if (!checkGridID(gridA) || !checkGridID(gridB))
    {
        LOG_ERROR("gridId error, gridId:" << gridA);
        return;
    }
    Rect rectA, rectB;
    getRectInRadius(&rectA, gridA, radius);
    getRectInRadius(&rectB, gridB, radius);
    for (int32_t nz = rectA.startZ; nz <= rectA.endZ; ++nz)
    {
        for (int32_t nx = rectA.startX; nx <= rectA.endX; ++nx)
        {
            if (!rectB.isContain(nx, nz))
            {
                continue;
            }
            GridID_T id = nx + nz * _w;
            if (!(_grids + id)->sweepAll(cb))
                return;
        }
    }
}

bool Zone::checkGridID(GridID_T gridId)
{
    return gridId >= 0 && gridId < _size;
}

bool Zone::checkX(int32_t x)
{
    return x >= 0 && x < _w;
}

bool Zone::checkZ(int32_t z)
{
    return z >= 0 && z < _h;
}

void Zone::getRectInRadius(Rect *rect, GridID_T gridId, int32_t radius)
{
    int32_t x = gridId % _w;
    int32_t z = gridId / _w;
    rect->startX = (x - radius > 0) ? (x - radius) : 0;
    rect->startZ = (z - radius > 0) ? (z - radius) : 0;
    rect->endX = (x + radius >= _w) ? _w - 1 : x + radius;
    rect->endZ = (z + radius >= _h) ? _h - 1 : z + radius;
}
