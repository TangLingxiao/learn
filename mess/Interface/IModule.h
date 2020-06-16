#pragma once

#include<cstring>
#define MODULE_NAME_LEN (64)

#define min(a, b) (a < b ? a : b)
class IKernel;

class IModule
{
public:
    virtual ~IModule() = default;
    virtual bool init(IKernel *kernel) = 0;
    virtual bool launch(IKernel *kernel) = 0;
    virtual bool destroy(IKernel *kernel) = 0;

public:
    inline void setNext(IModule *module){_next = module;}
    inline IModule* getNext(){return _next;}
    void setName(const char* name){memcpy(_name, name, min(MODULE_NAME_LEN - 1 ,strlen(name)));}
    const char* getName(){return _name;}
private:
    IModule *_next{nullptr};
    char _name[MODULE_NAME_LEN]{'\0'};
};

using getModuleFunc = IModule*(void);