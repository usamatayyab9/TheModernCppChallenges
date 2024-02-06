/**
 * @file 61_parallel_transform.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief
 * Compilation command : g++ -std=c++17 61_parallel_transform.cpp -lpthread
 * 
 *  This file is solution to "Problem 61. Parallel transform algorithm"
 *  mentioned in "Chapter 7: Concurrency" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *
 * Solution:
 * 
 * The solution is implemented using two functions `ParallelTransform` and
 * `Transform_n`. `ParallelTransform()` takes a cotainer and projection as input,
 * and applies projection function to each element on input container. See
 * functions comments for more details.
 * 
 * `Transform_n()` takes input a source iterator, destination iterator, an integer
 * denoting the number of element on which projection has to be applied and a projection
 * function. Applies projection function on source iterator elements and stores them
 * on destination iterator.
 * 
 * Driver code:
 * Initializes a vector of integers.
 * Applies a projection and saves projected vector.
 * Prints the original vector
 * Finally prints the projected vector.
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

using std::advance;
using std::begin;
using std::copy;
using std::cout;
using std::end;
using std::for_each;
using std::min;
using std::ostream_iterator;
using std::size;
using std::thread;
using std::transform;
using std::tuple;
using std::vector;

/**
 * @brief   Applies a unary function to the first n elements of a source range
 *          and stores the results in a destination range.
 * @tparam  SourceIteratorType Iterator type of the source range. This iterator type must be incrementable.
 * @tparam  DestinationIteratorType Iterator type of the destination range. This iterator type must be incrementable.
 * @tparam  UnaryFunction Type of the unary function to apply. This function type must be copyable.
 * @param src  - The beginning of the source range.
 * @param dest - The beginning of the destination range.
 * @param      - n The number of elements to transform.
 * @param projection - The unary function to apply to each element.
 */
template<class SourceIteratorType, class DestinationIteratorType , class UnaryFunction>
void Transform_n(SourceIteratorType src, DestinationIteratorType dest, size_t n, UnaryFunction projection)
{
    for (auto i = size_t{ 0 }; i < n; ++i, ++src, ++dest)
    {
        *dest = projection(*src);
    }
}


/**
 * @brief   Applies a unary function to the elements of a container in parallel.
 * @details If size of container is less than five then parallel operations are performed.
 *          The input container is divided into chunks of 5 and each is transformed
 *          on a different thread. After launching all thread function waits for all thread
 *          to finish. 
 * @tparam Container Type of the container.
 * @tparam UnaryFunction Type of the unary function to apply.
 * @param container  - The container to transform.
 * @param projection - The unary function to apply to each element.
 * @return             Copy of an input container such that each element is transformed using the
 *                     projection provided.
 */
template<class Container, class UnaryFunction>
auto ParallelTransform(Container container, UnaryFunction projection)
{
    constexpr auto kDivisionFactor = size_t{ 5 };
    if (size(container) < kDivisionFactor)
    {
        transform(begin(container), end(container), begin(container), projection);
    }
    else
    {
        auto threads       = vector<thread>{};
        auto iter          = begin(container);
        using IteratorType = decltype(iter);
        for (auto idx = size_t{ 0 }; idx < size(container); idx += kDivisionFactor)
        {
            threads.push_back(thread(
                &Transform_n<IteratorType, IteratorType, UnaryFunction>, 
                    iter, /* source */
                    iter, /* destination */
                    min(size(container) - idx, kDivisionFactor), /* No. of elements to transform starting from source, to cater when size of container is not divisible by 5 then remaining elements at end will not be exactly 5 */
                    projection /* function which will be applied */
            ));
            advance(iter, min(size(container) - idx, kDivisionFactor));
        }
        for (auto &t : threads) { t.join(); }
    }
    
    return container;
}

int main()
{
    auto vec = vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    auto projected_data = ParallelTransform(vec, [](auto i){ return i * i; });
    
    cout << "Original: ";
    copy(cbegin(vec), cend(vec), ostream_iterator<int>{ cout, " "});
    cout << "\n";

    cout << "After applying projection: ";
    copy(cbegin(projected_data), cend(projected_data), ostream_iterator<int>{ cout, " "});
    cout << "\n";
    return 0;
}