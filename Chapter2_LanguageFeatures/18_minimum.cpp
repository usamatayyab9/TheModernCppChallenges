/**
 * @file 18_minimum.cpp
 * @author Usama Tayyab (usamtayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 18_minimum.cpp
 * This file is solution to "Problem 18. Minimum function with any number of arguments"
 * mentioned in "Chapter 2: Language Features" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * "Write a function template that can take any number of arguments and returns the
 *  minimum value of them all, using operator < for comparison. Write a variant of this
 *  function template that can be parameterized with a binary comparison function to use
 *  instead of operator <."
 * 
 * This File implements 4 functions as required by the problem:
 * 1. A `Minimum()` function which has two overloads:
 *      1.1 First which takes two arguments of same type and returns minimum of the two
 *      1.2 Second which takes arbitrary number of arguments and returns minimum of all
 *          the elements given as argument. It uses a concept called variadic templates.
 * 2. Similary a `Minimum_if()` function which also has two overloads just like above
 *      Minimum() function. The only difference is that it takes an extrat arguement
 *      which is comparator object which is used to compare elements unlike Minimum()
 *      function which compares elementn using < operator.
 * 
 * Driver code:
 * Program demonstrates the use of  Minimum() and Minimum_if() function.
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <string>
#include <functional>

using std::cout;
using std::endl;
using std::string;

template <typename T> T Minimum(T arg1, T arg2) {
    if (arg1 < arg2) {
        return arg1;
    }
    return arg2;
}
template <typename First, typename ...Args>
auto Minimum(First first, Args... arguments)
{
    return Minimum(first, Minimum(arguments...));
}


template <class Comparator, typename T> T Minimum_if(Comparator comp, T arg1, T arg2) {
    if (comp(arg1, arg2)) {
        return arg1;
    }
    return arg2;
}
template <class Comparator, typename First, typename ...Args>
auto Minimum_if(Comparator comp, First first, Args... arguments)
{
    return Minimum_if(comp, first, Minimum_if(comp, arguments...));
}



int main()
{
    auto least   = Minimum(100, 2, 3, 4, 1, 0);
    auto maximum = Minimum_if(std::greater<int>{}, 100, 2, 3, 4, 1, 0);
    cout << least << ", " << maximum << endl;
    return 0;
}