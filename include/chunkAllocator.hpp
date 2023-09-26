#pragma once
#include <cassert>
#include <memory>
#include <map>
#include <algorithm>
template <typename T, std::size_t size = 10>
class chunkAlloc
{
private:
    size_t shift = 0;
    T *topPtr = nullptr;
    std::map<T*, std::size_t> chunks;
public:
    using value_type = T;

    T *allocate(size_t n)
    {
        if (shift + n > chunks.size() * size)
        {
            topPtr = reinterpret_cast<T*>(malloc(size * sizeof(T)));
            chunks[topPtr] = 0;
        }
        auto retPtr = topPtr + shift % size;
        shift += n;
        return retPtr;
    }

    void deallocate(T *p, size_t n)
    {
        const auto &chunk = find_if(chunks.begin(), chunks.end(), 
            [p](auto &item){return p - item.first < size;});
        assert(chunk != chunks.end());
        assert(chunk->second);
        if (!--(chunk->second))
        {
            free(chunk->first);
            chunks.erase(chunk);
        }
    }

    template <typename U, typename... Args>
    void construct(U *p, Args &&...args)
    {
        new (p) U(std::forward<Args>(args)...);
        const auto &chunk = find_if(chunks.begin(), chunks.end(), 
            [p](auto &item){return p - reinterpret_cast<U*>(item.first) < size;});
        assert(chunk != chunks.end());
        assert(chunk->second < size);
        (chunk->second)++;
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