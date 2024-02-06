/**
 * @file 56_select_algorithm.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * 
 * Compilation command : g++ -std=c++17 56_select_algorithm.cpp
 * 
 * This file is solution to "Problem 56. Select algorithm"
 *  mentioned in "Chapter 6: Algorithms and Data Structures" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *
 * Solution:
 * A `select` function which takes input a range , a projection function and a destination iterator.
 * Applies projectio function on indiviaudal elements of range and stores the result in
 * destination iterator.
 * 
 * Driver code:
 * - Initializes a list of books.
 * - Selects titles of the and prints them.
 * - Selects the authors of the prints them.
 * 
 * @copyright Copyright (c) 2024
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
using std::invoke_result_t;
using std::ostream_iterator;
using std::string;
using std::vector;

/**
 * @brief 
 * Loops over the range [first, last) and applies @param projection
 * on each element and storing result in the range starting at
 * @param d_iter. 
 * 
 * @tparam InputIteratorType 
 * @tparam OutputIterator 
 * @tparam Projection 
 * @param first 
 * @param last 
 * @param d_iter 
 * @param projection 
 * @return OutputIterator - position where last projection result is stored
 */
template <class InputIteratorType, class OutputIterator, class Projection>
OutputIterator Select(InputIteratorType first, InputIteratorType last, OutputIterator d_iter, Projection projection)
{
    while (first != last)
    {
        *d_iter = projection(*first);
        ++first;
    }
    return d_iter;
}

/**
 * @brief 
 * Transforms elements from the input range using a projection function.
 *  Returns a vector of projected elementes.
 * @tparam Container 
 * @tparam Projection 
 * @param c 
 * @param proj 
 * @return auto 
 */
template<class Container, class Projection>
auto Select(const Container &c, Projection proj)
{
    auto vec = vector<invoke_result_t<decltype(proj), typename Container::value_type>>{};
    Select(cbegin(c), cend(c), back_inserter(vec), proj);
    return vec;
}

struct Book
{
    int     id;
    string  title;
    string  author;
};

int main()
{
    auto kBooks = vector<Book> {
        { 101, "The C++ Programming Language"       , "Bjarne Stroustrup" },
        { 203, "Effective Modern C++"               , "Scott Meyers"      },
        { 404, "The Modern C++ Programming Cookbook", "Marius Bancila"    }
    };
    
    const auto kTitles = Select(kBooks, [](const Book &b) { return b.title; } );
    cout << "Titles: "; for (const auto &title : kTitles) { cout << title << '\n'; }

    const auto kAuthors = Select(kBooks, [](const Book &b) { return b.author; } );
    cout << "Authors: "; for (const auto &author : kAuthors) { cout << author << '\n'; }

    return 0;
}