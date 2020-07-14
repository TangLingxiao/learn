#pragma once
#include <cstdlib>
#include <mutex>

struct ILinkNode
{
    ILinkNode()
    {
    }
    void detach()
    {
        if (_prev)
        {
            _prev->_next = _next;
        }
        if (_next)
        {
            _next->_prev = _prev;
        }
    }
    ILinkNode *_prev{nullptr};
    ILinkNode *_next{nullptr};
};

struct LinkList
{
    ~LinkList()
    {
        clear();
    }

    void push_back(ILinkNode *node)
    {
        if (_head == nullptr)
        {
            _head = node;
            _tail = node;
        }
        else
        {
            node->_prev = _tail;
            _tail->_next = node;
            _tail = node;
        }
        _size++;
    }

    ILinkNode *pop_front()
    {
        if (_head == nullptr)
        {
            return nullptr;
        }
        ILinkNode *node = _head;
        _head = _head->_next;
        if (_head == nullptr)
        {
            _tail = nullptr;
        }
        else
        {
            _head->_prev = nullptr;
        }
        _size--;
        return node;
    }

    void erase(ILinkNode *node)
    {
        if (node == nullptr)
        {
            return;
        }
        node->_next->_prev = node->_prev;
        node->_prev->_next = node->_next;
        node->_next = node->_prev = nullptr;
    }

    bool empty()
    {
        return _head == nullptr;
    }

    void clear()
    {
        _head = _tail = nullptr;
        _size = 0;
    }

    ILinkNode *_head{nullptr};
    ILinkNode *_tail{nullptr};
    size_t _size{0};
};

template <typename T, typename L>
void mergeListByLock(T &to, T &from, L &lock)
{
    if (from.empty())
    {
        return;
    }
    std::lock_guard<L> guard(lock);
    if (to.empty())
    {
        to._head = from._head;
        to._tail = from._tail;
        to._size = from._size;
    }
    else
    {
        from._head->_prev = to._tail;
        to._tail->_next = from._head;
        to._tail = from._tail;
        to._size += from._size;
    }
    from._head = from._tail = nullptr;
    from._size = 0;
}
