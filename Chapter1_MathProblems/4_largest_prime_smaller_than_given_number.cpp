/**
 * @file 4_largest_prime_smaller_than_given_number.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 *          Compilation command : g++ -std=c++17 ./4_largest_prime_smaller_than_given_number.cpp
 * 
 *          This file is solution to "Problem 4: Largest prime smaller than given number"
 *          mentioned in "Chapter 1: Math Problems" of the book:
 *              - The Modern C++ Challenge(available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *          
 *          This file contains two functions IsPrime() and LargestPrimeSmallerThanN() see their comments section
 *          for their implemntation.
 *          Driver code:
 *          The program start by taking an input number(N) from the user, then computes the largest prime
 *          smaller than 'n' using function LargestPrimeSmallerThanN(). If no such prime exists it prints
 *          the message "No prime number exist smaller than <n>" otherwise prints the computed prime number.
 * 
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <cmath>
#include <utility>
#include <optional>

using std::cin;
using std::cout;
using std::endl;
using std::sqrt;
using std::pair;
using std::optional;

/**
 * @brief   Tests whether a given number is prime or not.
 * @details
 *          - Check # 1: Any number <= 1 is not a prime number.
 *          - Check # 2: Only even no. that is prime is 2 rest are all odd.
 *          - Traverses all numbers in range [3, sqrt(@param N)]. If N is divisible
 *            by any number in the range, then it considered not to be a prime.
 * @param   N
 * @return  true, if N is prime 
 * @return  false, if N is not a prime
 */
bool IsPrime(const auto &N)
{
    auto is_prime = false;
    if (N > 1) //1 is not prime number
    {
        if ((1 == (N & 1)) || (2 == N)) //even-odd test. Only odd numbers are prime, 2 is only prime number that is even
        {
            is_prime = true;
            for (auto [i, limit] = pair{ 3, sqrt(N) }; i <= limit ;i += 2)
            {
                if (0 == (N % i))
                {
                    is_prime = false;
                    break;
                }
            }
        }
    }
    return is_prime;
}

/**
 * @brief   Returns the largest prime that is smaller than @param N
 * @details
 *          - Since all numbers < 0 are not considered prime. So if N < 0,
 *            ouput for function will be an empty std::optional value.
 *          - Similary for N == 1 and N == 2, no primes exist of lesser value.
 *          - For N == 3, 2 is the answer as it is smaller than it and also prime number.
 *          - Search the range [N - 1, 1], and stop on the first prime found.  
 *          - NOTE: It is possible that N itself is prime number, and problem clearly states that
 *            result must be smaller than N. Hence in this case result can not be N. That is
 *            the reason search starts from N - 1.
 * @param   N 
 * @return  optional
 *          Result may or may not exist for values on @param N. See details 
 */
auto LargestPrimeSmallerThanN(auto N)
{
    auto result = optional<decltype(N)>{};
    if (N >= 3)
    {
        for (auto i = N - 1; i > 1 ;--i)
        {
            if (IsPrime(i)) {
                result = i;
                break;
            }
        }
    }
    return result;
}

int main()
{
    auto i = 0;
    cout << "Enter a number: ";
    cin >> i;

    auto result = LargestPrimeSmallerThanN(i);
    if (result)
    {
        cout << "Largest prime smaller than " << i << " is : " << result.value();
    }
    else
    {
        cout << "No prime number exist smaller than " << i;
    }
    return 0;
}