#include "poller.h"
#include "eventloop.h"

bool Poller::inLoopThread()
{
    return m_pLoop->inLoopThread();
}