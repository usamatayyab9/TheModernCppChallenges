/**
 * @file 6_abundant_numbers.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 *          Compilation command: g++ -std=c++17 6_abundant_numbers.cpp
 * 
 *          This file is solution to "Problem 6.  Abundant numbers"
 *          mentioned in "Chapter 1: Math Problems" of the book:
 *              - The Modern C++ Challenge(available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *         
 *          As per wikipedia abundant numbers are defined as:
 *              In number theory, an abundant number or excessive number
 *              is a positive integer for which the sum of its proper
 *              divisors is greater than the number. The integer 12 is the
 *              first abundant number. Its proper divisors are 1, 2, 3, 4 and 6
 *              for a total of 16. The amount by which the sum exceeds the number
 *              is the abundance. The number 12 has an abundance of 4.
 *          For more info, visit: https://en.wikipedia.org/wiki/Abundant_number
 *          
 *          Driver code: 
 *          The program starts by taking a number `N` as input from user via the console.
 *          Then prints all the abundant numbers from 1 to N.
 *          See function comments for implementation. 
 * 
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <utility>

using std::cin;
using std::cout;
using std::endl;
using std::pair;

/**
 * @brief 
 *      For reference proper divisor for a number is defined as:
 *          A positive divisor of n that is different from n is called a proper divisor.
 *          For more info, visit: https://en.wikipedia.org/wiki/Divisor
 *      
 *      Algorithm:
 *          N := Input parameter
 *          sum := 0, output parameter
 *          for i in range [1,N/2]:
 *              if N is divisible i:
 *                  add i to sum
 *          
 * @param N 
 * @return sum of all proper divisors of N
 */
auto SumOfProperDivisors(const auto N)
{
    auto sum = size_t{ 1 };
    for (auto [i, limit] = pair{ 2, N / 2 }; i <= limit ;++i)
    {
        if (0 == (N % i))
        {
            sum += i;
        }
    }
    return sum;
}

/**
 * @brief 
 *      Prints all abundant numbers and their abundance from 1 to @param N.
 *      First initializes a variable 'i' with value 1 with the same type as of `N`.
 *      Loop until i is less than equal to N.
 *      Initializes a variable 'sum' with sum of proper divisors of i using the
 *      function `SumOfProperDivisors()`.
 *      Then test if sum is greater than i, if yes it means i is an abundant number
 *      and print i and its abundance.
 * @param N 
 */
void PrintAbundantNumbersUptoN(const auto N)
{
    using integral_type = decltype(N);
    for (auto i = integral_type{ 1 }; i <= N ;++i)
    {
        auto sum = SumOfProperDivisors(i);
        if (sum > i)
        {
            cout << i << ", abundance = " << sum - i << endl;
        }
    }
}

int main()
{
    auto i = 0;
    cout << "Enter a number: ";
    cin >> i;

    cout << "Abundant numbers upto " << i << "are:\n";
    PrintAbundantNumbersUptoN(i);

    return 0;
}