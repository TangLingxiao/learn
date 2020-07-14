#pragma once

#include <stdlib.h>
#include "../Util/Util.h"
#include <cstring>
#include <algorithm>

class RingBuff
{
public:
    RingBuff(size_t size)
    {
        if (size & (size - 1))
        {
            size = RoundupPowOf2(size);
        }
        _buff = (char *)malloc(size);
        _size = size;
    }

    ~RingBuff()
    {
        free(_buff);
    }

    inline size_t Fls(size_t size)
    {
        size_t pos{0};
        for (size_t i = size >> 1; i != 0;)
        {
            i >>= 1;
            ++pos;
        }
        return pos + 1;
    }

    inline size_t RoundupPowOf2(size_t size)
    {
        return 1 << Fls(size - 1);
    }

    inline size_t CalcSize(size_t size)
    {
        if (size & (size - 1))
        {
            return RoundupPowOf2(size);
        }
        return size;
    }

    inline size_t Size() const { return _in - _out; }

    inline size_t TotalSize() const{return _size;}

    inline void In(const size_t size)
    {
        _in += size;
    }

    inline void Out(const size_t size)
    {
        LIB_ASSERT(_in - _out >= size, "wtf");
        _out += size;
    }

    inline void Clear()
    {
        _in = 0;
        _out = 0;
    }

    inline char *getCanWrite(size_t &size)
    {
        size_t freeSize = _size - (_in - _out);
        if (freeSize == 0)
        {
            return nullptr;
        }
        size_t realIn = _in & (_size - 1);
        size_t realOut = _out & (_size - 1);

        if (realIn >= realOut)
        {
            size = _size - realIn;
        }
        else
        {
            size = realOut - realIn;
        }
        return _buff + realIn;
    }

    inline char *getCanRead(size_t &size)
    {
        size_t useSize = _in - _out;
        if (useSize == 0)
        {
            return nullptr;
        }
        size_t realIn = _in & (_size - 1);
        size_t realOut = _out & (_size - 1);
        if (realIn <= realOut)
        {
            size = _size - realOut;
        }
        else
        {
            size = realIn - realOut;
        }
        return _buff + realOut;
    }

    inline bool Write(const void *content, const size_t size)
    {
        size_t freeSize = _size - (_in - _out);
        if (freeSize < size)
        {
            return false;
        }
        size_t realIn = _in & (_size - 1);
        if (size <= _size - realIn)
        {
            memcpy(_buff + realIn, content, size);
        }
        else
        {
            memcpy(_buff + realIn, content, _size - realIn);
            memcpy(_buff, (const char *)content + _size - realIn, size - (_size - realIn));
        }
        _in += size;
        return true;
    }

    inline size_t Read(void *buff, size_t len)
    {
        size_t useSize = _in - _out;
        if (useSize == 0)
        {
            return 0;
        }
        size_t readLen = std::min<size_t>(len, useSize);
        size_t realOut = _out & (_size - 1);
        if (_size - realOut >= readLen)
        {
            memcpy(buff, _buff + realOut, readLen);
        }
        else
        {
            memcpy(buff, _buff + realOut, _size - realOut);
            memcpy((char *)buff + _size - realOut, _buff, readLen - (_size - realOut));
        }
        _out += readLen;
        return readLen;
    }

    inline bool Empty()
    {
        return _in == _out;
    }

    inline void Realloc(size_t size)
    {
        if (size & (size - 1))
            size = RoundupPowOf2(size);

        size_t usedSize = _in - _out;
        if (usedSize > size)
            return;

        char *buffer = (char *)malloc(size);
        if (!buffer)
            return;

        if (usedSize > 0)
        {
            size_t realIn = _in & (_size - 1);
            size_t realOut = _out & (_size - 1);

            if (realIn > realOut)
                memcpy(buffer, _buff + realOut, usedSize);
            else
            {
                memcpy(buffer, _buff + realOut, _size - realOut);
                memcpy(buffer + _size - realOut, _buff, realIn);
            }
        }

        free(_buff);
        _buff = buffer;
        _out = 0;
        _in = usedSize;
        _size = size;
    }

private:
    char *_buff{nullptr};
    size_t _size{0};
    size_t _in{0};
    size_t _out{0};
};