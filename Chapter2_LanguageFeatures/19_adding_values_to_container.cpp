/**
 * @file 19_adding_values_to_container.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 19_adding_values_to_container.cpp 
 * This file is solution to "Problem 19. Adding a range of values to a container"
 * mentioned in "Chapter 2: Language Features" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * "Write a general-purpose function that can add any number of elements to the end of a 
 * container that has a method push_back(T&& value)."
 * 
 * This file implements two approaces for as a solution:
 * 1. A Pushback() which takes a  container and values which are to be inserted into container as an argumetn.
 * 2. A PushbackFold() functions takes the same argument as above but uses fold expression.
 *      
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

template <class Container, typename T>
void PushBack(Container &c, T first)
{
    c.push_back(first);
}

template <class Container, typename T, typename... Arguments>
void PushBack(Container &c, T first, Arguments... args)
{
    c.push_back(first);
    PushBack(c, args...);
}

template <class Container, typename T, typename... Arguments>
void PushBackFold(Container &c, T first, Arguments... args)
{
    c.push_back(first);
    (PushBackFold(c, args), ...);
}

int main()
{
    auto ivec = vector<int>{};
    PushBack(ivec, 1, 2, 3, 4, 5);
    PushBackFold(ivec, 6);
    PushBackFold(ivec, 7, 8);
    PushBackFold(ivec, 9, 10, 11);

    for (const auto &elem : ivec) { cout << elem << ' '; }

    return 0;
}