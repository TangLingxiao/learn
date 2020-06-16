#pragma once

class ICore
{
public:
    virtual ~ICore() = default;
    virtual bool ready() = 0;
    virtual bool init() = 0;
    virtual bool destroy() = 0;
};