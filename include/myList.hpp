#pragma once
#include <cstring>
#include <memory>

template <typename T, typename _Alloc = std::allocator<T>>
class myList
{
private:
    class Node
    {
    public:
        Node(const T &d)
        {
            data = d;
        }

        Node(T &&d)
        {
            std::swap(data, d);
        }

        template <typename... Args>
        Node(Args &&...args)
            : data(std::forward<Args>(args)...){};
        T data;
        Node *next = nullptr;
    };
    typename _Alloc::template rebind<Node>::other nodeAlloc;
    Node *head = nullptr;
    Node *tail = nullptr;

    void growTail(Node *ptr)
    {
        if (!head)
        {
            head = tail = ptr;
        }
        else
        {
            tail->next = ptr;
            tail = ptr;
        }
    }

public:
    class iterator
    {
    public:
        iterator(Node *ptr)
            : _ptr(ptr){};

        const T &operator*() const
        {
            return _ptr->data;
        }

        T &operator*()
        {
            return _ptr->data;
        }

        void operator++()
        {
            _ptr = _ptr->next;
        }
        bool operator!=(const iterator &rhs) const
        {
            return _ptr != rhs._ptr;
        }

    private:
        Node *_ptr;
    };

    void clear()
    {
        if (head)
        {
            auto toRemove = head;
            while (toRemove)
            {
                auto toRemoveNext = toRemove->next;
                nodeAlloc.destroy(toRemove);
                nodeAlloc.deallocate(toRemove, 1);
                toRemove = toRemoveNext;
            }
        }
        head = nullptr;
        tail = nullptr;
    }

    void operator=(myList &&rhs)
    {
        std::swap(head, rhs.head);
        std::swap(tail, rhs.tail);
        std::swap(nodeAlloc, rhs.nodeAlloc);
    }

    template <typename S>
    void operator=(S &&rhs)
    {
        clear();
        for (auto it = rhs.cbegin(); it != rhs.cend(); ++it)
        {
            auto ptr = nodeAlloc.allocate(1);
            nodeAlloc.construct(ptr, *it);
            growTail(ptr);
        }
    }

    template <typename... Args>
    void emplace_back(Args &&...args)
    {
        auto ptr = nodeAlloc.allocate(1);
        nodeAlloc.construct(ptr, std::forward<Args>(args)...);
        growTail(ptr);
    }

    iterator cbegin() const
    {
        if (head)
        {
            return head;
        }
        else
        {
            return nullptr;
        }
    }

    iterator begin()
    {
        return cbegin();
    }

    iterator cend() const
    {
        return nullptr;
    }

    iterator end()
    {
        return cend();
    }

    ~myList()
    {
        clear();
    }
};