#pragma once

#include <string.h>
#include <stdint.h>
#include "Util/Util.h"
template <uint8_t N = 2>
struct Vector
{
public:
    Vector()
    {
        memset(&x, 0, sizeof(x));
    }
    Vector(int32_t n)
    {
        for (uint8_t i = 0; i < N; ++i)
        {
            x[i] = n;
        }
    }
    Vector(const Vector &rh)
    {
        for (uint8_t i = 0; i < N; ++i)
        {
            x[i] = rh.x[i];
        }
    }
    Vector &operator=(const Vector &rh)
    {
        for (uint8_t i = 0; i < N; ++i)
        {
            x[i] = rh.x[i];
        }
        return *this;
    }
    constexpr inline uint8_t getDimension() { return N; }
    inline int32_t &operator[](uint8_t index)
    {
        LIB_ASSERT(index < N, "index error");
        return x[index];
    }
    Vector &operator+(const Vector &rh)
    {
        for (uint8_t i = 0; i < N; ++i)
        {
            x[i] += rh.x[i];
        }
        return *this;
    }
    Vector &operator-(const Vector &rh)
    {
        for (uint8_t i = 0; i < N; ++i)
        {
            x[i] -= rh.x[i];
        }
        return *this;
    }

    int64_t distance()
    {
        int64_t dis{0};
        for (uint8_t i = 0; i < N; ++i)
        {
            dis += x[i] * x[i];
        }
        return dis;
    }

    int64_t distance(const Vector &rh)
    {
        int64_t dis{0};
        for (uint8_t i = 0; i < N; ++i)
        {
            dis += (rh[i] - x[i]) * (rh[i] - x[i]);
        }
        return dis;
    }

public:
    int32_t x[N];
};

//在二维的情况下，我们称之为最小限定矩形。MBR(minimum bounding retangle)
//三维的情况下，我们称最新限定箱MBB(minimum bounding box)
template <uint8_t N = 2>
struct MBRect
{
public:
    MBRect()
    {
        memset(&leftbottom, 0, sizeof(leftbottom));
        memset(&righttop, 0, sizeof(righttop));
    }
    MBRect(const MBRect &rh)
    {
        memcpy(&leftbottom, &rh.leftbottom, sizeof(leftbottom));
        memcpy(&righttop, &rh.righttop, sizeof(righttop));
    }
    inline bool isContain(const MBRect &rh) const
    {
        for (uint8_t i = 0; i < N; ++i)
        {
            if (rh.righttop[i] < leftbottom[i] ||
                rh.leftbottom[i] > righttop[i])
            {
                return false;
            }
        }
        return true;
    }

public:
    Vector<N> leftbottom;
    Vector<N> righttop;
};

using Vector3D = Vector<3>;
using MBBox3D = MBRect<3>;