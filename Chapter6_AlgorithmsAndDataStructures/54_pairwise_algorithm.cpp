/**
 * @file 54_pairwise_algorithm.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 54_pairwise_algorithm.cpp
 *
 * This file is solution to "Problem 54. Pairwise algorithm"
 * mentioned in "Chapter 6: Algorithms and Data Structures" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *
 * Solution:
 * A function `PairWise` implements the desired solution. It has 2 overloads:
 * - One takes a container as an input and returns the vector of all possible
 *  adjacent pairs.
 * - Second overload takes iterators and copies all iterator pairs on destination
 * iterator.
 * 
 * See function comments for more details.  
 * 
 * Driver code:
 * - Initializes the vector with elements given as example in the book.
 * - Pass the vector to PairWise function.
 * - Finally prints all pair wise results. 
 *  
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <utility>
#include <vector>

using std::back_inserter;
using std::cout;
using std::cbegin;
using std::cend;
using std::endl;
using std::pair;
using std::vector;

/**
 * @brief Creates pairs of adjacent elements from the given range.
 *
 * @tparam InputIterator Type of the input iterator.
 * @tparam OutputIterator Type of the output iterator.
 * @param first Iterator pointing to the beginning of the input range.
 * @param last Iterator pointing to the end of the input range.
 * @param d_iter Iterator for the output range where pairs will be stored.
 * @return OutputIterator pointing to the end of the output range.
 *
 * @details
 * The `PairWise` function takes a range defined by the iterators `first` and `last`
 * and creates pairs of adjacent elements. The resulting pairs are stored in the
 * output range defined by the iterator `d_iter`. If the input range has an odd
 * number of elements, the last element is not paired with any other element.
 */
template<class InputIterator, class OutputIterator>
OutputIterator PairWise(InputIterator first, InputIterator last, OutputIterator d_iter)
{
    while (first != last)
    {
        auto val1 = *first; ++first;
        if (first == last) { break; }
        auto val2 = *first; ++first;

        *d_iter = pair{ val1, val2 };
        ++d_iter;
    }
    return d_iter;
}

/**
 * @brief Creates pairs of adjacent elements from a container.
 *
 * @tparam Container Type of the input container.
 * @param c Reference to the input container.
 * @return Vector of pairs representing adjacent elements in the input container.
 *
 * @details
 * The `PairWise` function takes a container `c` and creates pairs of adjacent elements.
 * The resulting pairs are stored in a vector, and the vector is returned.
 * If the input container has an odd number of elements, the last element is not paired
 * with any other element.
 */
template<class Container>
auto PairWise(const Container &c)
{
    using ValueType = typename Container::value_type;
    auto vec = vector<pair<ValueType, ValueType>>{};
    PairWise(cbegin(c), cend(c), back_inserter(vec));
    return vec;
}

int main()
{
    auto v      = vector{ 1, 1, 3, 5, 8, 13, 21 };
    auto result = PairWise(v);
    for (auto const &[first, second] : result)
    {
        cout << '{' << first << ',' << second << '}' << endl;
    }
    return 0;
}