/**
 * @file 20_any_all_none.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 *  Compilation command: g++ -std=c++17 20_any_all_none.cpp
 * 
 * This file is solution to "Problem 20. Container any, all, none"
 * mentioned in "Chapter 2: Language Features" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * "Write a set of general-purpose functions that enable checking whether any, all, or none of
 *   the specified arguments are present in a given container. These functions should make it
 *   possible to write code as follows:
 *       std::vector<int> v{ 1, 2, 3, 4, 5, 6 };
 *       assert(contains_any(v, 0, 3, 30));
 *
 *       std::array<int, 6> a{ { 1, 2, 3, 4, 5, 6 } };
 *       assert(contains_all(a, 1, 3, 5, 6));
 *
 *       std::list<int> l{ 1, 2, 3, 4, 5, 6 };
 *       assert(!contains_none(l, 0, 6));"
 * 
 * This file implements four different function as a solution.
 * 1. A contains() function which takes a container and single value as an argument and tests whether 
 *      the value exists in container or not.
 * 2. A containsAny() function which takes a container and arbitrary number of values as an argument and tests whether 
 *      ANY of the value exists in container or not using contains() and fold expressions.
 * 3. A containsAll() function which takes a container and arbitrary number of values as an argument and tests whether 
 *      ALL the values exists in container or not using contains() and fold expressions.
 * 4. A containsNone() function which takes a container and arbitrary number of values as an argument and tests whether 
 *      NONE of the values exists in container or not using contains() and fold expressions.
 * 
 * Driver code:
 *  Initializes a vector with some values.
 *  Then calls all of the ContainsAny(), ContainsAll() and ContainsNone() on vector with some values.
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <array>
#include <string>
#include <list>
using std::cbegin;
using std::cend;
using std::cout;
using std::endl;
using std::find;
using std::vector;
using std::array;
using std::string;
using std::list;

/**
 * @brief Check for the existance of value in a container
 * 
 * @tparam Container 
 * @tparam T 
 * @param c 
 * @param value 
 * @return true - if the @param value exists in @param c
 * @return false - otherwise
 */
template <class Container, class T>
bool Contains(const Container &c, const T &value)
{
    return find(cbegin(c), cend(c), value) != cend(c);
}

template <class Container, typename First, typename... Arguments>
bool ContainsAny(const Container &c, const First &first, const Arguments& ...arguments)
{
    return Contains(c, first) || (ContainsAny(c, arguments) || ...);
}

template <class Container, typename First, typename... Arguments>
bool ContainsAll(const Container &c, const First &first, const Arguments& ...arguments)
{
    return Contains(c, first) && (ContainsAll(c, arguments) && ...);
}

template <class Container, typename First, typename... Arguments>
bool ContainsNone(const Container &c, const First &first, const Arguments& ...arguments)
{
    return !ContainsAny(c, first, arguments...);
}

int main()
{
    auto ivec = vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    cout << "vector: ";
    std::copy(ivec.cbegin(), ivec.cend(), std::ostream_iterator<int>{ cout, " "});
    cout << endl;

    cout << "Contains any of 50, 100, 200 : " << std::boolalpha << ContainsAny(ivec, 50, 100, 200) << endl; // should return false
    cout << "Contains any of 1, 50, 200   : " << std::boolalpha << ContainsAny(ivec, 1, 50, 200) << endl; // should return true
    cout << endl;

    cout << "Contains all of 4, 5, 6   : " << std::boolalpha << ContainsAll(ivec, 4, 5, 6) << endl; // should return true
    cout << "Contains all of 4, 5, 600 : " << std::boolalpha << ContainsAll(ivec, 4, 5, 600) << endl; // should return false
    cout << endl;

    cout << "Contains none of 7, 8, 9       : " << std::boolalpha << ContainsNone(ivec, 7, 8, 9) << endl; // should return false
    cout << "Contains none of 700, 800, 900 : " << std::boolalpha << ContainsNone(ivec, 700, 800, 900) << endl; // should return false
    
    return 0;
}