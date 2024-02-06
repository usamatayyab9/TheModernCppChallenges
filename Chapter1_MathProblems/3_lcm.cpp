/**
 * @file 3_lcm.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 *          Compilation command: g++ -std=c++17 3_lcm.cpp
 *          This file is solution to "Problem 3 : Least common multiple" of book
 *          The Modern C++ Challenge(available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *          The lcm is calculted using gcd. gcd function is same as in problem 2(2_gcd.cpp).
 *          This file provides two functions for calculating least common multiple(lcm):
 *          1. LCM which calculates the lcm of two numbers.
 *          2. LCMOfMany which calculates lcm of multiples numbers.
 * 
 *          Driver code:
 *          The program start by initializing a vector with two elements 4 and 6.
 *          Then prints these elements, the caculates their lcm using LCMOfMany function
 *          and finally prints the result.
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>
#include <iterator>
#include <type_traits>

using std::cin;
using std::cout;
using std::endl;
using std::abs;
using std::accumulate;
using std::cbegin;
using std::cend;
using std::copy;
using std::ostream_iterator;
using std::vector;

/**
 * @brief 
 *          Uses Euclidean algorihtm for computing greatest common divisor of two elements.
 * @param   a 
 * @param   b 
 * @return  Greatest common divisor of a and b 
 */
auto GCD(const auto &a, const auto &b)
{
    if (0 == b) { return a; }

	return GCD(b, a % b);
}

auto LCM(const auto &a, const auto &b)
{
    if ((0 == a) && (0 == b))
    {
        return 0;
    }
    auto gcd_a_b = GCD(a, b);
    return abs(a) * (abs(b) / gcd_a_b);
}

template<class Container>
auto LCMOfMany(const Container &c)
{
    //Extract the arithmetic type of elements of c.
    using container_value_type = typename std::remove_reference_t<std::remove_const_t<decltype(c)>>::value_type;
    
    auto lcm = container_value_type{ 1 };
    for (const auto &elem : c)
    {
        lcm = LCM(lcm, elem);
    }
    return lcm;
}

int main()
{
    auto elements = vector{4, 6};
    cout << "LCM of ";
    for (const auto &elem : elements)
    {
        cout << elem << " ";
    }
    auto i = LCMOfMany(elements);
    cout << " is " << LCMOfMany(elements);
    
    return 0;
}