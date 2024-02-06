/**
 * @file 55_zip.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 54_pairwise_algorithm.cpp
 * 
 *  This file is solution to "Problem 55. Zip algorithm"
 *  mentioned in "Chapter 6: Algorithms and Data Structures" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *
 * Solution:
 * A function `Zip` with two overloads is provided:
 * - One overload takes iterators and copies zipped elements to destination iterator.
 * - Second overload takes two containers and returns vector of pair.
 * 
 * Driver code:
 * - Initliazes different containers with different element types.
 * - Uses the function ZipAndPrint to print zipped elements.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <string>

using std::back_inserter;
using std::copy;
using std::cout;
using std::cbegin;
using std::cend;
using std::endl;
using std::is_same_v;
using std::list;
using std::ostream_iterator;
using std::pair;
using std::string;
using std::vector;

/**
 * @brief Combines elements from two ranges into pairs.
 * 
 * @details
 * The `Zip` function combines elements from two input ranges defined by iterators
 * `first1` to `last1` and `first2` to `last2`. The resulting pairs are stored in
 * the output range defined by the iterator `d_iter`. If either input range has more
 * elements than the other, the excess elements are ignored.
 *
 * @tparam IteratorTypeRange1 Type of the first input range iterator.
 * @tparam IteratorTypeRange2 Type of the second input range iterator.
 * @tparam OutputIteratorType Type of the output iterator.
 * @param first1    Iterator pointing to the beginning of the first input range.
 * @param last1     Iterator pointing to the end of the first input range.
 * @param first2    Iterator pointing to the beginning of the second input range.
 * @param last2     Iterator pointing to the end of the second input range.
 * @param d_iter    Iterator for the output range where pairs will be stored.
 * 
 * @return          OutputIteratorType pointing to the end of the output range.
 */
template <class IteratorTypeRange1, class IteratorTypeRange2, class OutputIteratorType>
OutputIteratorType Zip(IteratorTypeRange1 first1, IteratorTypeRange1 last1,
                      IteratorTypeRange2 first2, IteratorTypeRange2 last2,
                      OutputIteratorType d_iter)
{
    while ((first1 != last1) && (first2 != last2))
    {
        *d_iter = pair{ *first1, *first2 };
        ++first1;
        ++first2;
        ++d_iter;
    }
    return d_iter;
}

/**
 * @brief Combines elements from two ranges into pairs.
 * 
 * @details
 * The `Zip` function combines elements from two input ranges `r1` and `r2` into pairs.
 * The resulting pairs are stored in a vector, and the vector is returned. If either input
 * range has more elements than the other, the excess elements are ignored.
 *
 * @tparam Range1 Type of the first input range.
 * @tparam Range2 Type of the second input range.
 * @param r1 Reference to the first input range.
 * @param r2 Reference to the second input range.
 * 
 * @return Vector of pairs representing combined elements from the input ranges.
 */
template <class Range1, class Range2>
auto Zip(const Range1 r1, const Range2 &r2)
{
    using Range1ValueType = typename Range1::value_type;
    using Range2ValueType = typename Range2::value_type;
    auto vec = vector<pair<Range1ValueType, Range2ValueType>>{};
    Zip(cbegin(r1), cend(r1), cbegin(r2), cend(r2), back_inserter(vec));
    return vec;
}

void ZipAndPrint(const auto &r1, const auto &r2)
{
    cout << "Range 1: { "; for (const auto &elem : r1) { cout << elem << ", "; } cout << "}\n";
    cout << "Range 2: { "; for (const auto &elem : r2) { cout << elem << ", "; } cout << "}\n";
    const auto kZipped = Zip(r1, r2);
    cout << "Zipped : [ ";
    for (const auto &[first, second] : kZipped)
    {
        cout << "{ " << first << ',' << second << " }" << ',';
    }
    cout << '\n';
}

int main()
{
    {
        auto v1     = vector{ 1, 3, 5, 7, 9, 11, 13, 15, 17 };
        auto v2     = vector{ 2, 4, 6, 8, 10 };
        ZipAndPrint(v1, v2);
    }
    {
        auto vi     = vector{ 1, 2, 3, 4, 5 };
        auto vs     = vector<string>{ "One", "Two", "Three", "Four", "Five" };
        ZipAndPrint(vi, vs);
    }
    {
        auto ls     = list<string>{ "Six", "Seven", "Eight", "Nine", "Ten" };
        auto vi     = vector{ 6, 7, 8, 9, 10 };
        ZipAndPrint(ls, vi);
    }
    return 0;
}