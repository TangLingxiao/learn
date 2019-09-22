#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "logmgr.h"

class App
{
public:
    virtual ~App() {}
    virtual bool init()
    {
        return LogMgr::getInstance().init("test.log", "./", true);
    }
    virtual void loop() {}
    virtual void fini()
    {
        LogMgr::getInstance().release();
    }

    int main(int argc, char **argv)
    {
        if (!init())
        {
            exit(-1);
        }
        loop();
        fini();
        return 0;
    }
};
#endif