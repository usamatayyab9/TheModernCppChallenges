/**
 * @file 8_armstrong_numbers.cpp
 * @author Usama Tayyab (usamtayyab9@gmail.com)
 * @brief 
 *      Compilation command: g++ -std=c++17 8_armstrong_numbers.cpp
 *      
 *      This file is solution to "Problem 8. Armstrong numbers"
 *      mentioned in "Chapter 1: Math Problems" of the book:
 *      - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *      
 *      The problem statement is:
 *      "Write a program that prints all Armstrong numbers with three digits."
 * 
 *      Armstrong numbers:
 *      In number theory, a narcissistic number (also known as a pluperfect digital invariant (PPDI),
 *      an Armstrong number(after Michael F. Armstrong) or a plus perfect number) in a given number
 *      base b is a number that is the sum of its own digits each raised to the power of the number of digits.
 *      For example taken from the book:
 *      As an example, the smallest Armstrong number is 153, which is equal to 1^3 + 5^3 + 3^3 = 153.
 * 
 *      This file provides three functions.
 *      1. Print3DigitArmstrongNumbers() which prints all the 3-digit numbers which are armstrong numbers.
 *      2. PrintNumbersUptoN_if() which prints all the numbers from 0 to N(specified as parametr) which
 *          fulfill a certain criteria(also speceified as parameter)
 *      3. IsArmstrong() which takes a number as input and return either true or false indicating whther 
 *          input number is armstrong or not.
 *      
 *      Driver code:
 *      The program first prints all 3-digit armstrong numbers. The prins all armstrong number upto
 *      a certain number specified by vairable.
 *      See function comments for implementation details.
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;
using std::pow;
using std::log10;
using std::floor;

/**
 * @brief Prints all 3-digit Armstrong numbers.
 *      For definition of armstrong number see comments above.
 *      This function iterates through all possible 3-digit numbers (100 to 999)
 *      and prints those that are Armstrong numbers. The function calculates the
 *      sum of cubes of individual digits for each number and checks if it equals
 *      the number itself. If so, the number is an Armstrong number and is printed
 *      to the console.
 */
void Print3DigitArmstrongNumbers()
{
    for (auto hundereds_place = 1; hundereds_place <= 9; ++hundereds_place)
    {
        for (auto tens_place = 0; tens_place <= 9; ++tens_place)
        {
            for (int units_place = 0; units_place <= 9; ++units_place)
            {
                const auto kNumber            = hundereds_place * 100 + tens_place * 10 + units_place;
                const auto kSumOfCubeOfDigits = pow(hundereds_place, 3) + pow(tens_place, 3) + pow(units_place, 3);
                if (kNumber == kSumOfCubeOfDigits)
                {
                    cout << kNumber << "\n";
                }
            }
        }
    }
}

/**
 * @brief 
 *      Prints all th numbers from 0 to N for which the
 *      predicate @param comp returns true.
 * @tparam Comparator 
 * @param N 
 * @param comp 
 */
template<typename Comparator>
void PrintNumbersUptoN_if(const size_t &N, Comparator comp)
{
    for (auto i = size_t{ 0 }; i < N ;++i)
    {
        if (comp(i))
        {
            cout << i << "\n";
        }
    }
}

/**
 * @brief Test whether the input number @param N is an armstrong number or not.
 *  
 *      First test the base check i.e. if the number is between [0,9], return true.
 *      For numbers greater than 9:
 *      - Calculates the digit count using log10
 *      - initializes a sum variable with 0
 *      - For each digit of @param N calculate its power to digit count.
 *      - Add to sum.
 *      - Once no digits are left test sum == N.      
 * @param N 
 * @return true if input number is an armstrong
 * @return false otherwise
 */
bool IsArmstrong(const size_t &N)
{
    auto is_armstrong = false;
    if (N >= 0 && N <= 9) { is_armstrong = true; }
    else
    {
        const auto kDigitCount = floor(log10(N) + 1);
        auto sum               = size_t{ 0 };
        for (auto n = N; 0 != n ;n /= 10)
        {
            sum += pow(n % 10, kDigitCount);
        }
        is_armstrong = (N == sum);
    }
    return is_armstrong;
}

int main()
{
    cout << "3 digit Armstrong numbers are:\n";
    Print3DigitArmstrongNumbers();

    auto limit = 10000;
    cout << "Armstrong numbers upto " << limit << " are:\n";
    PrintNumbersUptoN_if(limit, IsArmstrong);

    return 0;
}