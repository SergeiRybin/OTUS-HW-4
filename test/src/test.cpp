#define BOOST_TEST_MODULE static library test // Defines the name of the program which is used in the messages
#include <boost/test/unit_test.hpp>
#include <chunkAllocator.hpp>
#include <hard.hpp>
#include <myList.hpp>

BOOST_AUTO_TEST_SUITE(MainSuite)

BOOST_AUTO_TEST_CASE(copyTest)
{
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
    myList<hard, chunkAlloc<hard>> tempMyList{};
    myList<hard, std::allocator<hard>> targetDiffAllocMyList{};

    targetSameAllocMyList = allocMyList;
    tempMyList = allocMyList;

    for (auto lhs = allocMyList.cbegin(), rhs = targetSameAllocMyList.cbegin(); lhs != allocMyList.cend();)
    {
        BOOST_TEST((*lhs).fa == (*rhs).fa);
        BOOST_TEST((*lhs).fi == (*rhs).fi);
        ++rhs;
        ++lhs;
    }

    {
        auto lhs = targetSameAllocMyList.cbegin();
        (*lhs).fa = 99;
        BOOST_TEST((*allocMyList.begin()).fa == 0);
    }

    targetSameAllocMyList = std::move(tempMyList);

    for (auto lhs = allocMyList.cbegin(), rhs = targetSameAllocMyList.cbegin(); lhs != allocMyList.cend();)
    {
        BOOST_TEST((*lhs).fa == (*rhs).fa);
        BOOST_TEST((*lhs).fi == (*rhs).fi);
        ++rhs;
        ++lhs;
    }

    {
        auto lhs = targetSameAllocMyList.cbegin();
        (*lhs).fa = 100;
        BOOST_TEST((*allocMyList.begin()).fa == 0);
    }

    targetDiffAllocMyList = allocMyList;

    auto drhs = targetDiffAllocMyList.cbegin();
    for (auto lhs = allocMyList.cbegin(); lhs != allocMyList.cend();)
    {
        BOOST_TEST((*lhs).fa == (*drhs).fa);
        BOOST_TEST((*lhs).fi == (*drhs).fi);
        ++drhs;
        ++lhs;
    }


    targetDiffAllocMyList = std::move(targetDiffAllocMyList);
    drhs = targetDiffAllocMyList.cbegin();
    for (auto lhs = allocMyList.cbegin(); lhs != allocMyList.cend();)
    {
        BOOST_TEST((*lhs).fa == (*drhs).fa);
        BOOST_TEST((*lhs).fi == (*drhs).fi);
        ++drhs;
        ++lhs;
    }
}

BOOST_AUTO_TEST_CASE(multipleChunkTest)
{
    myList<hard, chunkAlloc<hard>> allocMyList{};
    for (size_t i = 0; i < 12; ++i)
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

    targetSameAllocMyList = std::move(allocMyList);
}

BOOST_AUTO_TEST_SUITE_END()