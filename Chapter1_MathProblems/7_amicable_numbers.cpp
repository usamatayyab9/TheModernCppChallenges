/**
 * @file 7_amicable_numbers.cpp
 * @author Usama Tayyab (usamatayyab@gmail.com)
 * @brief
 *      Compilation command: g++ -std=c++17 7_amicable_numbers.cpp
 * 
 *      This file is solution to "Problem 7. Amicable numbers"
 *      mentioned in "Chapter 1: Math Problems" of the book:
 *      - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *      
 *      The problem statement is:
 *      "Write a program that prints the list of all pairs of amicable numbers smaller than 1,000,000."
 *      
 *      As expalined briefly in the book:
 *      "Two numbers are said to be amicable if the sum of the proper divisors of one number is
 *      equal to that of the other number. The proper divisors of a number are the positive prime
 *      factors of the number other than the number itself. Amicable numbers should not be
 *      confused with friendly numbers. For instance, the number 220 has the proper divisors 1, 2, 4,
 *      5, 10, 11, 20, 22, 44, 55, and 110, whose sum is 284. The proper divisors of 284 are 1, 2, 4, 71,
 *      and 142; their sum is 220. Therefore, the numbers 220 and 284 are said to be amicable."
 * 
 *      Since it is stated in the problem that only amicable numbers upto 1,000,000 are required, this
 *      limit is expressed by constant variable `kLimit`.
 *      This file contains two methods of generating amicable numbers. One method generates amicable
 *      numbers at run-time while the other generates at compile-time. Two functions `SumOfProperDivisors()`
 *      and `PrintAmicableNumbers()` in accordance with each other generates amicable numbe at run-time.
 *      Where as for compile-time functionality it is implemented in namespace `AmicablesNumbersConstepxr`.
 *      
 *      @note  constexpr version is tested for kLimit=10000. Hence compiling for values greater than
 *      10000 may cause the compilation to be temrinated because gcc only allows a certain number
 *      of compile time computations. However this value can be icreased using the flag:
 *      -fconstexpr-ops-limit=<value>. For the sake of compiling this file and running it
 *      constexpr function AmicablesNumbersConstepxr::PrintAmicablesNumbersFastUpto1000000() is commented.
 * 
 *      See function comments for implementation details.
 * 
 *      Driver code:
 *      Program prints all amicable numbers upto `kLimit`.
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <array>
#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>
#include <unordered_set>
#include <cmath>
#include <type_traits>
#include <chrono>

using std::array;
using std::cin;
using std::cout;
using std::endl;
using std::size;
using std::cbegin;
using std::begin;
using std::cend;
using std::vector;
using std::pair;
using std::sqrt;
using std::transform;
using std::unordered_set;
using std::chrono::steady_clock;
using std::chrono::milliseconds;
using std::chrono::duration_cast;

inline constexpr auto kLimit = 1'000'000;

/**
 * @brief 
 *      Calculates the sum of all proper divisors of @param N.
 *      Iterates through potential divisors in the range [2,sqrt(N)].
 *      Adds them to the sum if they are divisors. Additionally, it
 *      checks if the corresponding divisor (N/i) is different from
 *      the current divisor (i) and if it is also a divisor of N,
 *      adding it to the sum as well. This approach ensures that each
 *      divisor is added only once to the sum, including the cases
 *      where N/i is a divisor.
 * @param N 
 * @return sum of all proper divisors of @param N. 
 */
auto SumOfProperDivisors(const auto N)
{
    auto sum = size_t{ 1 };
    for (auto [i, limit] = pair{ 2, sqrt(N) }; i <= limit ;++i)
    {
        if (0 == (N % i))
        {
            sum += i;
            /*! It might be possible that N/i is also divisible by N.
                Now there are two cases:
                1. For instance N = 12, i = 2:
                    then, 12 % 2 == 0, add 2 to sum
                    also 12 / 2 = 6 and 12 % 6 == 0, add 6 to sum too.
                2. For instance N = 36, i = 6:
                    then 36 % 6 == 0, add 6 to sum
                    also 36 / 6 = 6 and 36 % 6 == 0, this time dont add 6 again.
            */
           if (auto second_divisior = N / i; second_divisior != i && (0 == N % second_divisior))
           {
               sum += second_divisior;
           }
        }
    }
    return sum;
}

/**
 * @brief 
 *      Prints all the amicable number from 2 to @param limit.
 * @param limit 
 */
void PrintAmicableNumbers(const size_t &limit)
{
    auto uset = unordered_set<size_t>{}; //For storing which elements are already printed
    for (auto i = size_t{ 2 }; i < limit ;++i)
    {
        if (uset.find(i) == cend(uset))
        {
            if (const auto kSum = SumOfProperDivisors(i); kSum < limit && kSum != i)
            {
                if (auto kSum2 = SumOfProperDivisors(kSum); kSum2 == i)
                {
                    uset.insert(kSum);
                    cout << "(" << i << "," << kSum << ")\n";
                }
            }
        }
    }
}


namespace AmicablesNumbersConstepxr
{
    constexpr auto MySqrt(const auto x)
    {
        auto r = std::integral_constant<decltype(x), 1>::value;
        while ((r * r) < x) { ++r; }
        return (r * r) == x ? r : r - 1;
    }

    constexpr auto SumOfProperDivisors(const auto N)
    {
        auto sum = size_t{ 1 };
        for (auto [i, limit] = pair{ 2, MySqrt(N) }; i <= limit ;++i)
        {
            if (0 == (N % i))
            {
                sum += i;
                /*! It might be possible that N/i is also divisible by N.
                    Now there are two cases:
                    1. For instance N = 12, i = 2:
                        then, 12 % 2 == 0, add 2 to sum
                        also 12 / 2 = 6 and 12 % 6 == 0, add 6 to sum too.
                    2. For instance N = 36, i = 6:
                        then 36 % 6 == 0, add 6 to sum
                        also 36 / 6 = 6 and 36 % 6 == 0, this time dont add 6 again.
                */
                if (auto second_divisior = N / i; second_divisior != i && (0 == N % second_divisior))
                {
                    sum += second_divisior;
                }
            }
        }
        return sum;
    }

    constexpr auto GetListOfSumOfProperDivisors()
    {
        auto proper_divisors_sum_array = array<int, kLimit + 1>{};
        proper_divisors_sum_array[0] = 0;
        for (auto idx = size_t{ 1 }; idx < size(proper_divisors_sum_array) ;++idx)
        {
            proper_divisors_sum_array[idx] = AmicablesNumbersConstepxr::SumOfProperDivisors(idx); 
        }
        return proper_divisors_sum_array;
    }

    void PrintAmicablesNumbersFastUpto1000000()
    {
        /*
        constexpr auto kSumOfPrpoerDivisorsArray = AmicablesNumbersConstepxr::GetListOfSumOfProperDivisors();
        for (auto idx = size_t{ 1 }; idx < size(kSumOfPrpoerDivisorsArray) ;++idx)
        {
            const auto kSum = kSumOfPrpoerDivisorsArray[idx];
            if (idx != kSum && kSum < size(kSumOfPrpoerDivisorsArray) && kSumOfPrpoerDivisorsArray[kSum] == idx)
            {
                cout << idx << " , " << kSum << "\n";
            }
        }
        */
    }
}

int main()
{
    cout << "Amicable numbers upto " << kLimit << " are:\n";
    {
        auto start_timepoint = steady_clock::now();
        PrintAmicableNumbers(kLimit);
        auto duration = duration_cast<milliseconds>(steady_clock::now() - start_timepoint);
        cout << "Generating amicable numbers took(Runtime): " << duration.count() << "ms\n";
    }
    {
        /**
         * This block of code attempts to generate amicable numbers upto 1'000'000 at compile
         * time using constexpr functions. But fails to do so because only limited amount of 
         * computations can be performed at compile time, for kLimit = 1'000'000 no. of computations
         * exceed the default limit allowed.
         * However it is tested that for kLimit = 1'00'00, the code compiles successfully
         * and generates output much faster than above block of code which performs all evaluations
         * at run time.
         * To enable this functionality uncomment the function
         * AmicablesNumbersConstepxr::PrintAmicablesNumbersFastUpto1000000().
         */
        auto start_timepoint = steady_clock::now();
        AmicablesNumbersConstepxr::PrintAmicablesNumbersFastUpto1000000();
        auto duration = duration_cast<milliseconds>(steady_clock::now() - start_timepoint);
        cout << "Generating amicable numbers took(compiletime): " << duration.count() << "ms\n";
    }
    return 0;
}