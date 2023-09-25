#include <iostream>
#include <map>

#include <chunkAllocator.hpp>
#include <hard.hpp>
#include <myList.hpp>

int main(int argc, char const *argv[])
{
    try
    {
        std::map<int, hard> simpleMap{};
        for (size_t i = 0; i < 10; ++i)
        {
            simpleMap.emplace(std::piecewise_construct, std::forward_as_tuple(i), std::forward_as_tuple(i, i));
        }
        std::cout << "======simpleMap======" << std::endl;
        for (const auto &i : simpleMap)
        {
            std::cout << i.first << ": fa=" << i.second.fa << " fi=" << i.second.fi << std::endl;
        }

        std::map<int, hard, std::less<int>, chunkAlloc<std::pair<const int, hard>>> allocMap{};
        for (size_t i = 0; i < 10; ++i)
        {
            allocMap.emplace(std::piecewise_construct, std::forward_as_tuple(i), std::forward_as_tuple(i, i));
        }
        std::cout << "======allocMap======" << std::endl;
        for (const auto &i : allocMap)
        {
            std::cout << i.first << ": fa=" << i.second.fa << " fi=" << i.second.fi << std::endl;
        }

        myList<hard, chunkAlloc<hard>> allocMyList{};
        for (size_t i = 0; i < 10; ++i)
        {
            allocMyList.emplace_back(i, i);
        }
        std::cout << "======allocMyList======" << std::endl;
        size_t c = 0;
        for (const auto &i : allocMyList)
        {
            std::cout << c << ": fa=" << i.fa << " fi=" << i.fi << std::endl;
            ++c;
        }

        myList<hard, chunkAlloc<hard>> targetSameAllocMyList{};
        myList<hard, std::allocator<hard>> targetDiffAllocMyList{};

        targetSameAllocMyList = allocMyList;
        targetSameAllocMyList = std::move(allocMyList);

        targetDiffAllocMyList = allocMyList;
        targetDiffAllocMyList = std::move(allocMyList);

        std::cout << "======targetDiffAllocMyList======" << std::endl;
        c = 0;
        for (const auto &i : targetDiffAllocMyList)
        {
            std::cout << c << ": fa=" << i.fa << " fi=" << i.fi << std::endl;
            ++c;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}