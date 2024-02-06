/**
 * @file 63_parallel_min_max_async.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 63_parallel_min_max_async.cpp -lpthread
 * 
 *  This file is solution to "Problem 63. Parallel min and max element algorithms using asynchronous functions"
 *  mentioned in "Chapter 7: Concurrency" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *
 * Solution: 
 * 
 * Two functions are implemented for computing minimum and maximum elements
 * in a parallel. These two functions are `MinElem_async()` and `MaxElem_async()`.
 * 
 * Driver code:
 * Initializes a vector ints.
 * Then uses MinElem_async() and MaxElem_async() to compute the minimum and maximum
 * elements respectively. 
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <utility>
#include <future>
#include <thread>

using std::advance;
using std::async;
using std::begin;
using std::copy;
using std::cout;
using std::distance;
using std::end;
using std::endl;
using std::future;
using std::max_element;
using std::min;
using std::min_element;
using std::ostream_iterator;
using std::pair;
using std::size;
using std::vector;

/**
 * @brief Finds the minimum element in a range using asynchronous tasks.
 * @details This function divides the input range into segments and uses asynchronous tasks to find
 *          the minimum element in each segment concurrently. Then, it compares the minimums of each
 *          segment to determine the overall minimum in the entire range.
 *
 * @tparam IteratorType Type of the iterators.
 * @tparam Comparator Type of the comparator function.
 * @param first Iterator pointing to the start of the range.
 * @param last Iterator pointing to the end of the range.
 * @param comp Comparator function for element comparison. Default is std::less<>.
 * @return Iterator pointing to the minimum element in the range.
 */
template <class IteratorType, typename Comparator = std::less<>>
IteratorType MinElem_async(IteratorType first, IteratorType last, Comparator comp = Comparator{})
{
    constexpr auto kParallelismFactor = size_t{ 5 };
    const auto kSize = distance(first, last);
    auto result = first;
    if (kSize < kParallelismFactor)
    {
        result = min_element(first, last, comp);
    }
    else
    {
        const auto kTotalThreads = kSize / kParallelismFactor;
        auto tasks               = vector<future<IteratorType>>{};
        for (auto iter = size_t{ 0 }; iter < kSize ; iter += kParallelismFactor)
        {
            const auto kIncrement = min(kSize - iter, kParallelismFactor);
            tasks.push_back(async(min_element<IteratorType, Comparator>, first, first + kIncrement, comp
            ));
            advance(first, kIncrement);
        }

        auto min_iterators = vector<IteratorType>{};
        for (auto &task : tasks)
        {
            // Collect the results from asynchronous tasks.
            min_iterators.push_back(task.get());
        }
        // Find the overall minimum by comparing the minimums of each segment.
        auto pos = std::min_element(begin(min_iterators), end(min_iterators), [comp](const auto &current, const auto &smallest)
        {
            return comp(*current, *smallest);
        });
        result = *pos;
    }
    return result;
}

/**
 * @brief Finds the maximum element in a range using asynchronous tasks.
 * @details This function divides the input range into segments and uses asynchronous tasks to find
 *          the maximum element in each segment concurrently. Then, it compares the maximums of each
 *          segment to determine the overall maximum in the entire range.
 *
 * @tparam IteratorType Type of the iterators.
 * @tparam Comparator Type of the comparator function.
 * @param first Iterator pointing to the start of the range.
 * @param last Iterator pointing to the end of the range.
 * @param comp Comparator function for element comparison. Default is std::less<>.
 * @return Iterator pointing to the maximum element in the range.
 */
template <class IteratorType, typename Comparator = std::less<>>
IteratorType MaxElem_async(IteratorType first, IteratorType last, Comparator comp = Comparator{})
{
    constexpr auto kParallelismFactor = size_t{ 5 };
    const auto kSize = distance(first, last);
    auto result = first;
    if (kSize < kParallelismFactor)
    {
        result = max_element(first, last, comp);
    }
    else
    {
        const auto kTotalThreads = kSize / kParallelismFactor;
        auto tasks               = vector<future<IteratorType>>{};
        for (auto iter = size_t{ 0 }; iter < kSize ; iter += kParallelismFactor)
        {
            const auto kIncrement = min(kSize - iter, kParallelismFactor);
            tasks.push_back(async(max_element<IteratorType, Comparator>, first, first + kIncrement, comp
            ));
            advance(first, kIncrement);
        }

        auto max_iterators = vector<IteratorType>{};
        for (auto &task : tasks)
        {
            max_iterators.push_back(task.get());
        }
        // Find the overall maximum by comparing the maximums of each segment.
        auto pos = std::max_element(begin(max_iterators), end(max_iterators), [comp](const auto &largest, const auto &current)
        {
            return comp(*largest, *current);
        });
        result = *pos;
    }
    return result;
}

int main()
{
    auto vec = vector<int>{ 3, 2, 6, 5, 0, -3, 100, 1, 4, 9, 8, 10, 1 };

    auto min_elem_pos = MinElem_async(begin(vec), end(vec));
    cout << "Minimum element: " << *min_elem_pos << endl;

    auto max_elem_pos = MaxElem_async(begin(vec), end(vec));
    cout << "Maximum element: " << *max_elem_pos << endl;

    return 0;
}