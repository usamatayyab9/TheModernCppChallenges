/**
 * @file 9_primes_factors.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 *      Compilation command: g++ -std=c++17 9_primes_factors.cpp
 *      This file is solution to "Problem 9.  Prime factors of a number"
 *      mentioned in "Chapter 1: Math Problems" of the book:
 *      - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *      
 *      The problem statement is:
 *      "Write a program that prints the prime factors of a number entered by the user."
 * 
 *      The solution is implented using a struct PrimeFactorsGenerator which generates all prime
 *      factors of a number return them as vector and a function PrintPrimeFactors() which uses
 *      uses the above struct to obtain a list of prime factors and then prints them.
 *      
 *      Driver code:
 *      The program first take a number as input from user via the console. Pass it to the function
 *      PrintPrimeFactors() which prints all of its prime factors.
 *      Then Prints all the prime factors of 600'851'475'143 using the same function. Printing prime
 *      factors of 600'851'475'143 is given as an further exercise.
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>

using std::cbegin;
using std::cend;
using std::cin;
using std::copy;
using std::cout;
using std::endl;
using std::ostream_iterator;
using std::sqrt;
using std::vector;

/**
 * @brief A structure for generating prime factors of a given number.
 */
struct PrimeFactorsGenerator
{
    /**
     * @brief 
     *          Divides N by d unitl it is no longer perfectly divisible by d.
     *          For each perfect division of N by d, add d into @param prime_factors
     *          Returns the updated N.
     * @param   N 
     * @param   d 
     * @param[in,out]   prime_factors 
     * @return[out]  size_t 
     */
    size_t DivideAndUpdateFatorsList(size_t N, const size_t &d, vector<size_t> &prime_factors)
    {
        while (0 == (N % d))
        {
            prime_factors.push_back(d);
            N /= d;
        }
        return N;
    }
    public:
        /**
     * @brief Returns a list of all prime factors of the given number N_arg.
     * 
     *      The function uses trial division to find all prime factors of the input number.
     *      It iterates through potential divisors starting from 2, dividing N by each prime factor
     *      until N is no longer divisible by that factor. Since 2 is the only even number that is
     *      prime so it is handled initially,  The process start with 3 continues with odd numbers until
     *      N becomes 1. Any remaining value of N greater than 1 after the loop is also a prime factor.
     * 
     * @param N_arg - The number for which prime factors are to be generated.
     * @return A vector containing all the prime factors of the input number.
     */
    vector<size_t> operator()(const size_t &N_arg)
    {
        auto prime_factors  = vector<size_t>{};
        auto N              = DivideAndUpdateFatorsList(N_arg, 2, prime_factors);
        const auto kSqrtOfN = sqrt(N);
        for (auto d = 3; (1 != N) && (d <= kSqrtOfN) ;d += 2)
        {
            N = DivideAndUpdateFatorsList(N, d, prime_factors);
        }
        /*! If N is not 1 it means it is prime a number*/
        if (N > 1) { prime_factors.push_back(N); }

        return prime_factors;
    }
};

void PrintPrimeFactors(const auto &i)
{
    const auto kPrimeFactors = PrimeFactorsGenerator{}(i);
    cout << "Prime factors of " << i << " are :\n";
    for (const auto &prime_factor : kPrimeFactors)
    {
        cout << prime_factor << ' ';
    }
    cout << '\n';
}

int main()
{
    auto i = size_t{ 0 };
    cout << "Enter a number : ";
    cin >> i;
    
    PrintPrimeFactors(i);
    constexpr auto kLargeNumber = 600'851'475'143;
    PrintPrimeFactors(kLargeNumber);
    
    return 0;
}