/**
 * @file 5_sexy_prime_numbers.cpp
 * @author Usama Tayyab (usamatayyab@gmail.com)
 * @brief 
 *          Compilation command : g++ -std=c++17 5_sexy_prime_numbers.cpp
 *          This file is solution to "Problem 5. Sexy prime pairs"
 *          mentioned in "Chapter 1: Math Problems" of the book:
 *              - The Modern C++ Challenge(available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *          
 *          For reference sexy primes are prime numbers which differ by 6.
 *          For example following are some sexy primes:
 *              - 5 and 11
 *              - 7 and 13
 *              - 11 and 17
 *              - 13 and 19
 *          
 *          This file contains two functions IsPrime() and PrintAllSexyPrimesUptoN(). See function definition for more details.
 *          Driver code:
 *          Program start by taking a number input from user and then prints all sexy primes upto it.
 *  
 * @copyright Copyright (c) 2023
 */
#include <iostream>
#include <utility>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;
using std::pair;
using std::sqrt;

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

void PrintAllSexyPrimesUptoN(const auto N)
{
    /*!
        Since 2 can not form a sexy prime with 8 so search begins with 3.
    */
    constexpr auto three = std::integral_constant<decltype(N), 3>::value; //For avoiding integral type issues i.e. overflow
    for (auto i = three; i <= N ;i += 2)
    {
        if (IsPrime(i) && IsPrime(i + 6))
        {
            cout << i << " , " << i + 6 << endl;
        }
    }
}

int main()
{
    auto i = 0;
    cout << "Enter a number: ";
    cin >> i;

    cout << "List of sexy primes upto " << i << " are : \n";
    PrintAllSexyPrimesUptoN(i);

    return 0;
}