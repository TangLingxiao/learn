#pragma once

class IKernel
{
public:
    virtual ~IKernel() = default;
    //log
    virtual void syncLog(const char*) = 0;
    virtual void asyncLog(const char*) = 0;
    
    //net

    //timer

};