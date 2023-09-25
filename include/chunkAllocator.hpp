#pragma once
#include <cassert>
#include <memory>

template <typename T, std::size_t size = 10>
class chunkAlloc
{
private:
    size_t shift = 0;
    void *basePtr = nullptr;

public:
    using value_type = T;

    T *allocate(size_t n)
    {
        assert((shift + n) <= size);
        if (!basePtr)
        {
            basePtr = malloc(size * sizeof(T));
        }
        auto retPtr = reinterpret_cast<T *>(basePtr) + shift;
        ++shift;
        return retPtr;
    }

    void deallocate(T *p, size_t n)
    {
        if (shift)
        {
            --shift;
        }
        else
        {
            free(basePtr);
        }
    }

    template <typename U, typename... Args>
    void construct(U *p, Args &&...args) const
    {
        new (p) U(std::forward<Args>(args)...);
    }

    void destroy(T *p) const
    {
        p->~T();
    }

    template <typename U>
    struct rebind
    {
        using other = chunkAlloc<U>;
    };
};