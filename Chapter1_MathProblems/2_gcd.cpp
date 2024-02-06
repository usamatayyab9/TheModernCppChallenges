/**
 * @file 2_gcd.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief
 *          Compilation command: g++ -std=c++17 2_gcd.cpp
 *          This file serves as the solution to Problem 2, which involves calculating gcd
 *          of two numbers. It corresponds to Chapter 1's Math Problems in the book "The Modern C++ Challenge"
 *          (available on Amazon: https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861). 
 *          
 *          Driver code:
 *          Program start by taking two numbers from user as input on the console.
 *          Then prints their greatest common divisor(gcd).  
 * 
 * 
 * @copyright Copyright (c) 2023
 */
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

/**
 * @brief 
 *          Uses Euclidean algorihtm for computing greatest common divisor of two elements.
 * @param   a 
 * @param   b 
 * @return  Greatest common divisor of a and b 
 */
auto GCD(const size_t &a, const size_t &b)
{
    if (0 == b) { return a; }

	return GCD(b, a % b);
}

int main()
{
    auto a = size_t{ 0 };
    auto b = size_t{ 0 };

    cout << "Enter first number  : "; cin >> a;
    cout << "Enter second number : "; cin >> b;
    
    cout << "gcd(" << a << "," << b << ") = " << GCD(a, b);
    
    return 0;
}