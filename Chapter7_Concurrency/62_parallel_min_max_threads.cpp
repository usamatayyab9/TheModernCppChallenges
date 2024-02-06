/**
 * @file 62_parallel_min_max_threads.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 62_parallel_min_max_threads.cpp -lpthread
 * This file is solution to "Problem 62. Parallel min and max element algorithms using threads"
 *  mentioned in "Chapter 7: Concurrency" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *
 * Solution:
 * Two functions are implemented for computing minimum and maximum elements
 * in a parallel. These two functions are `MinElem_mt()` and `MaxElem_mt()`.
 * 
 * Driver code:
 * Initializes a vector ints.
 * Then uses MinElem_mt() and MaxElem_mt() to computed minimum and maximum
 * elements respectively. 
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <algorithm>
#include <thread>
#include <vector>
#include <iterator>
#include <utility>
#include <cmath>
#include <chrono>

using std::advance;
using std::begin;
using std::cbegin;
using std::cend;
using std::copy;
using std::cout;
using std::distance;
using std::end;
using std::endl;
using std::min;
using std::minmax_element;
using std::ostream_iterator;
using std::pair;
using std::size;
using std::thread;
using std::vector;

/**
 * @brief   Finds the minimum element in a range using multiple threads.
 * @details 
 *          This function divides the input range into segments and employs multiple threads to find
 *          the minimum element in each segment concurrently. Then, it compares the minimums of each
 *          segment to determine the overall minimum in the entire range. Multiple thread are only 
 *          utilized if number of elements is greater than `kParallelismFactor`. Lets say the 
 *          input range contains 12 elements then threading will be done in following manner:
 *          T1 : Calculates minimum position of first 5 elements
 *          T2 : Calculates minimum position of second 5 elements
 *          T3 : Calculates minimum position of last 2 elements
 *          All thread that are laucnhed are stored in a vector. Once all the threads are launched
 *          main thread wait for all thread to fininsh executing.
 *          
 * 
 * @tparam IteratorType Type of the iterators.
 * @tparam Comparator Type of the comparator function.
 * @param first Iterator pointing to the start of the range.
 * @param last Iterator pointing to the end of the range.
 * @param comp Comparator function for element comparison. Default is std::less<>.
 * @return Iterator pointing to the minimum element in the range.
 */
template <class IteratorType, typename Comparator = std::less<>>
IteratorType MinElem_mt(IteratorType first, IteratorType last, Comparator comp = Comparator{})
{
    constexpr auto kParallelismFactor = size_t{ 5 };
    const auto kSize = distance(first, last);
    auto result = first;
    if (kSize < kParallelismFactor)
    {
        result = std::min_element(first, last, comp);
    }
    else
    {
        const auto kTotalThreads = kSize / kParallelismFactor;
        auto minimums_vec        = vector<IteratorType>(kTotalThreads); // vector where each thread will stores its result, 1st thread will stores at index 0, 2nd thread will store at index 1 and soon 
        auto threads             = vector<thread>{}; // vector to store all threads
        for (auto iter = size_t{ 0 }; iter < kTotalThreads ;++iter)
        {
            threads.push_back(thread{
                [start = first, end = (iter == kTotalThreads - 1) ? last : first + kParallelismFactor,
                comp, &result = minimums_vec[iter]]()
                {
                    // Find the minimum element in the segment.
                    result = std::min_element(start, end, comp);
                }
            });
            advance(first, kParallelismFactor);
        }
        for (auto &t : threads) { t.join(); }
        // Find the overall minimum by comparing the minimums of each segment.
        auto pos = std::min_element(begin(minimums_vec), end(minimums_vec), [comp](const auto &current, const auto &smallest)
        {
            return comp(*current, *smallest);
        });
        result = *pos;
    }
    return result;
}

/**
 * @brief   Finds the maximum element in a range using multiple threads.
 * @details 
 *          This function divides the input range into segments and employs multiple threads to find
 *          the maximum element in each segment concurrently. Then, it compares the maximum of each
 *          segment to determine the overall maximum in the entire range. Multiple thread are only 
 *          utilized if number of elements is greater than `kParallelismFactor`. Lets say the 
 *          input range contains 12 elements then threading will be done in following manner:
 *          T1 : Calculates maximum position of first 5 elements
 *          T2 : Calculates maximum position of second 5 elements
 *          T3 : Calculates maximum position of last 2 elements
 *          All thread that are laucnhed are stored in a vector. Once all the threads are launched
 *          main thread wait for all thread to fininsh executing.
 *          
 * 
 * @tparam IteratorType Type of the iterators.
 * @tparam Comparator Type of the comparator function.
 * @param first Iterator pointing to the start of the range.
 * @param last Iterator pointing to the end of the range.
 * @param comp Comparator function for element comparison. Default is std::less<>.
 * @return Iterator pointing to the maximum element in the range.
 */
template <class IteratorType, typename Comparator = std::less<>>
IteratorType MaxElem_mt(IteratorType first, IteratorType last, Comparator comp = Comparator{})
{
    constexpr auto kParallelismFactor = size_t{ 5 };
    const auto kSize = distance(first, last);
    auto result = first;
    if (kSize < kParallelismFactor)
    {
        result = std::max_element(first, last, comp);
    }
    else
    {
        const auto kTotalThreads = kSize / kParallelismFactor;
        auto maximums_vec        = vector<IteratorType>(kTotalThreads);// vector where each thread will stores its result, 1st thread will stores at index 0, 2nd thread will store at index 1 and soon 
        auto threads             = vector<thread>{};// vector to store all threads
        for (auto iter = size_t{ 0 }; iter < kTotalThreads ;++iter)
        {
            threads.push_back(thread{
                [start = first, end = (iter == kTotalThreads - 1) ? last : first + kParallelismFactor,
                comp, &result = maximums_vec[iter]]()
                {
                    result = std::max_element(start, end, comp);
                }
            });
            advance(first, kParallelismFactor);
        }
        for (auto &t : threads) { t.join(); }
        // Find the overall maximum by comparing the maximums of each segment.
        auto pos = std::max_element(begin(maximums_vec), end(maximums_vec), [comp](const auto &largest, const auto &current)
        {
            return comp(*largest, *current);
        });
        result = *pos;
    }
    return result;
}

int main()
{
    auto vec = vector<int>{ -3, 2, 6, 5, 0, 1, 4, 9, 8, 10, 1, 100 };

    auto min_elem_pos = MinElem_mt(cbegin(vec), cend(vec));
    cout << "Minimum element: " << *min_elem_pos << endl;

    auto max_elem_pos = MaxElem_mt(cbegin(vec), cend(vec));
    cout << "Maximum element: " << *max_elem_pos << endl;
    return 0;
}