#pragma once

#include "Structure/List.h"
#include "Util/TimeHelper.h"
#include "TimerMgr.h"
#include <functional>

#define TVN_BITS (6)
#define TVR_BITS (8)
#define TVN_SIZE (1 << TVN_BITS)
#define TVR_SIZE (1 << TVR_BITS)
#define TVN_MASK (TVN_SIZE - 1)
#define TVR_MASK (TVR_SIZE - 1)
#define TVEC_NUM (4)
#define JIFFIES_TIME (10)
#define INDEX(N) ((this->timer_jiffies >> (TVR_BITS + (N)*TVN_BITS)) & TVN_MASK)

struct tvec
{
    LinkList vec[TVN_SIZE];
};

struct tvec_root
{
    LinkList vec[TVR_SIZE];
};

struct TimerBase : public ILinkNode
{
    uint64_t timer_id;
    int64_t expires;
    uint32_t count;
    int64_t interval;
    std::function<void()> callback;
};

class TimerWheel
{
public:
    TimerWheel()
    {
        _start_time = UtilTime::getNowMS();
    }

    void addTimer(TimerBase *timer)
    {
        uint64_t expires = timer->expires;
        uint64_t jiffies = getJiffies(expires);
        uint64_t idx = jiffies - this->timer_jiffies;
        LinkList *vec;
        if (idx < TVR_SIZE)
        {
            int i = jiffies & TVR_MASK;
            vec = this->tv_root.vec + i;
        }
        else if (idx < 1 << (TVR_BITS + TVN_BITS))
        {
            int i = (jiffies >> TVR_BITS) & TVN_MASK;
            vec = this->tvs[0].vec + i;
        }
        else if (idx < 1 << (TVR_BITS + 2 * TVN_BITS))
        {
            int i = (jiffies >> (TVR_BITS + TVN_BITS)) & TVN_MASK;
            vec = this->tvs[1].vec + i;
        }
        else if (idx < 1 << (TVR_BITS + 3 * TVN_BITS))
        {
            int i = (jiffies >> (TVR_BITS + 2 * TVN_BITS)) & TVN_MASK;
            vec = this->tvs[2].vec + i;
        }
        else if ((signed long)idx < 0)
        {
            vec = this->tv_root.vec + (this->timer_jiffies & TVR_MASK);
        }
        else
        {
            int i;
            if (idx > 0xffffffffUL)
            {
                idx = 0xffffffffUL;
                jiffies = idx + this->timer_jiffies;
            }
            i = (jiffies >> (TVR_BITS + 3 * TVN_BITS)) & TVN_MASK;
            vec = this->tvs[3].vec + i;
        }
        vec->push_back(timer);
    }

    int cascade(struct tvec *tv, int index)
    {
        /*cascade all the timers from tv to up one level*/
        LinkList *tv_list = tv->vec + index;
        TimerBase *tmp = (TimerBase *)tv_list->pop_front();
        while (tmp)
        {
            addTimer(tmp);
            tmp = (TimerBase *)tv_list->pop_front();
        }
        return index;
    }

    void runTimer()
    {
        _current_time = UtilTime::getNowMS();
        auto jiffies = getJiffies(_current_time);
        while (jiffies >= this->timer_jiffies)
        {
            int index = this->timer_jiffies & TVR_MASK;
            if (!index &&
                (!cascade(this->tvs, INDEX(0))) &&
                (!cascade(this->tvs + 1, INDEX(1))) &&
                (!cascade(this->tvs + 2, INDEX(2))))
            {
                cascade(this->tvs + 3, INDEX(3));
            }
            ++this->timer_jiffies;
            auto work_list = this->tv_root.vec + index;
            TimerBase *temp = (TimerBase *)work_list->pop_front();
            while (temp)
            {
                temp->callback();
                if(--temp->count <= 0)
                {
                    TimerMgr::getInstance().recycleTimer(temp);
                    temp = (TimerBase *)work_list->pop_front();
                    continue;
                }
                temp->expires = UtilTime::getNowMS() + temp->interval;
                addTimer(temp);
                temp = (TimerBase *)work_list->pop_front();
            }
        }
    }

    uint64_t getJiffies(int64_t timeMs)
    {
        return uint64_t(timeMs - _start_time) / JIFFIES_TIME;
    }

private:
    int64_t _start_time{0};
    int64_t _current_time{0};
    uint64_t timer_jiffies{0};
    tvec_root tv_root;
    tvec tvs[TVEC_NUM];
};
