/**
 * @file 57_quicksort.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 57_quicksort.cpp
 * 
 * This file is solution to "Problem 57. Sort algorithm"
 *  mentioned in "Chapter 6: Algorithms and Data Structures" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *
 * Solution:
 * To implement the quick sort alogrithm followinf functions are provided:
 * - A `Partition` function.
 * - And two overloads of `QuickSort`. One overload uses < operator by
 *  default and second overload takes an extra argument which is the 
 * comparator function provided by caller. 
 * 
 * Driver code:
 * - Initializes a vector with some random elements.
 * - Sorts the vector using default comparison method(< operator).
 * - Prints the vector.
 * - Sorts the vector again using std::greater.
 * - Prints the vector.
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <iterator>
#include <vector>
#include <cassert>
#include <algorithm>
#include <functional>

using std::begin;
using std::cout;
using std::endl;
using std::greater;
using std::is_sorted;
using std::less;
using namespace std::placeholders;
using std::swap;
using std::vector;

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
void QuickSort(IteratorType first, IteratorType last, Comparator comp)
{
    if (first != last)
    {
        const auto kPivot = *first;
        auto pos = Partition(first, last, bind(comp, std::placeholders::_1, kPivot));
        QuickSort(first, pos, comp);
        
        /*!
            If pivot element is already at correct position.
            Consider the scenario:
            comp  = less or less_equal
            [first, last) = -3,1,3,-2 then pos == 0th index
            So next call should be from pos + 1 othwerwise it would be from pos 
        */
        QuickSort(pos + (pos == first), last, comp);
    }
}

template <class IteratorType>
void QuickSort(IteratorType first, IteratorType last)
{
    QuickSort(first, last, less{});
}

int main()
{
    auto vec = vector<int>{ 3, 6, 2, 7, 8, 9, 0, 4, 5, -1, -3, -3 };
    cout << "Before sorting" << endl;
    for (const auto &kElem : vec) { cout << kElem << " "; } cout << endl;
    
    QuickSort(begin(vec), end(vec));
    cout << "After sorting with less as criteria" << endl;
    for (const auto &kElem : vec) { cout << kElem << " "; } cout << endl;

    QuickSort(begin(vec), end(vec), greater{});
    cout << "After sorting with greater as criteria" << endl;
    for (const auto &kElem : vec) { cout << kElem << " "; } cout << endl;
    

    return 0;
}