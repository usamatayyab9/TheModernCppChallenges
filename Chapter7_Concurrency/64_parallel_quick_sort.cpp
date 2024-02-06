/**
 * @file 64_parallel_quick_sort.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command :  g++ -std=c++17 64_parallel_quick_sort.cpp -lpthread
 *
 *  This file is solution to "Problem 64. Parallel sort algorithm"
 *  mentioned in "Chapter 7: Concurrency" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution: 
 * The functions `QuickSort_mt()` which has two overloads one that takes only iteratores
 * and other overload that takes a comparator function as an additional argument.
 * For paritioning the `Partition()` function is used by `QuickSort_mt()`
 * to partition the array.
 *  
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <iterator>
#include <vector>
#include <cassert>
#include <algorithm>
#include <functional>
#include <thread>

using std::begin;
using std::cout;
using std::distance;
using std::endl;
using std::greater;
using std::is_sorted;
using std::less;
using namespace std::placeholders;
using std::swap;
using std::thread;
using std::vector;

/**
 * @brief Partitions a range based on a predicate using the Hoare partition scheme.
 *
 * @tparam IteratorType Type of the iterators.
 * @tparam Comparator Type of the comparator function.
 * @param first Iterator pointing to the start of the range.
 * @param last Iterator pointing to the end of the range.
 * @param comp Comparator function for element comparison.
 * @return Iterator pointing to the partition point.
 */
template <class IteratorType, class Comparator>
IteratorType Partition(IteratorType first, IteratorType last, Comparator comp)
{
    auto prev = first;
    for (; prev != last && comp(*prev); ++prev) {}

    for (auto next = prev; next != last ;++next)
    {
        if (comp(*next))
        {
            swap(*prev, *next);
            ++prev;
        }
    }
    return prev;
}


template <class IteratorType, class Comparator>
void QuickSort_mt(IteratorType first, IteratorType last, Comparator comp)
{
    if (first != last)
    {
        const auto kPivot = *first;
        // Partition the range and get the iterator to the partition point.
        auto pos = Partition(first, last, bind(comp, std::placeholders::_1, kPivot));
        const auto kSize = distance(first, last);
        // If the size is small, execute sequentially.
        if (kSize < 10000)
        {
            // cout << "Executing sequentially\n";
            QuickSort_mt(first, pos, comp);
        
            /*!
                If pivot element is already at correct position.
                Consider the scenario:
                comp  = less or less_equal
                [first, last) = -3,1,3,-2 then pos == 0th index
                So next call should be from pos + 1 othwerwise it would be from pos 
            */
            QuickSort_mt(pos + (pos == first), last, comp);
        }
        else
        {
            auto t1 = thread{ QuickSort_mt<IteratorType, Comparator>, first, pos, comp };
            auto t2 = thread{ QuickSort_mt<IteratorType, Comparator>, pos + (pos == first), last, comp };
            t1.join();
            t2.join();
        }
    }
}

template <class IteratorType>
void QuickSort_mt(IteratorType first, IteratorType last)
{
    QuickSort_mt(first, last, less{});
}

int main()
{
    auto vec = vector<int>{ 3, 6, 2, 7, 8, 9, 0, 4, 5, -1, -3, -3 };
    cout << "Before sorting" << endl;
    for (const auto &kElem : vec) { cout << kElem << " "; } cout << endl;
    
    QuickSort_mt(begin(vec), end(vec));
    cout << "After sorting with less as criteria" << endl;
    for (const auto &kElem : vec) { cout << kElem << " "; } cout << endl;

    QuickSort_mt(begin(vec), end(vec), greater{});
    cout << "After sorting with greater as criteria" << endl;
    for (const auto &kElem : vec) { cout << kElem << " "; } cout << endl;

    return 0;
}