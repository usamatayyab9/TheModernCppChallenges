/**
 * @file 1_sum_of_numbers_divisile_by_3_and_5.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief
 *          Compilation command: g++ -std=c++17 1_sum_of_numbers_divisible_by_3_and_5.cpp
 *          
 *          This file serves as the solution to Problem 1, which involves finding the sum of
 *          natural numbers divisible by both 3 and 5. It corresponds to Chapter 1's Math
 *          Problems in the book "The Modern C++ Challenge"
 *          (available on Amazon: https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861).
 *          
 *          The file presents three solutions:
 *          1. The function `SumOfNaturalNumbers_if_n()` takes two input parameters and
 *                  returns the sum of integers for which a given predicate is true.
 *                  Its time complexity is O(N); refer to function comments for details.
 *          2. `SumOfNaturalNumbersDivisibleBy3And5` is a struct with a function operator
 *                  overload that calculates the desired result using a series of multiplications
 *                  and additions instead of iterating over every element from 1 to N.
 *                  Its time complexity is O(1); see struct comments.
 *          3. `Soln()` represents the solution from the book. It has been included for result
 *                  comparison and to measure the execution time of all approaches.
 * 
 *          The driver code operates as follows:
 *          The program initiates by obtaining a number input from the user via the console.
 *          It then computes the sum of all natural numbers divisible by 3 and 5 up to N
 *          using the three approaches described above, measuring their respective execution durations.
 *          Subsequently, an assertion verifies that the results from all three approaches are identical.
 *          The program then prints the results and execution times of each method on separate lines.
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <chrono>
#include <cassert>

using std::cin;
using std::cout;
using std::endl;
using std::chrono::steady_clock;
using namespace std::chrono_literals;

/**
 * @brief 
 *      Iterates all the numbers from 1 to @param n and accumulates all the numbers
 *      for which predicate @param comp returns true. If no number from 1 to n
 *      fulfill the predicate then returns 0.
 * @tparam Comparator 
 * @param n 
 * @param comp 
 * @return  - Sum of all natural numbers for which the predicated comp returns true.    
 */
template<class Comparator>
auto SumOfNaturalNumbers_if_n(const size_t &n, Comparator comp)
{
    auto sum = size_t{ 0 };
    for (auto i = size_t{ 1 }; i < n ;++i) {
        if (comp(i)) {
            sum += i;
        }
    }
    return sum;
}

/**
 * @brief 
 *      Calculates the sum of all natural number from 1 to n that are divisible by 3 and 5.
 *      'n' is specified in function call operator.
 */
struct SumOfNaturalNumbersDivisileBy3And5
{
    private:
    constexpr size_t NumberOfMultiplesUpto(const size_t &n, const size_t &x)
    {
        return (n - 1) / x;
    }

    constexpr size_t SumOfFirstNaturalNumbers(const size_t &n)
    {
        return (n * (n + 1)) / 2;
    }
    
    public:
    constexpr auto operator()(const size_t &n)
    {
        const auto kTotalNoOfMultiplesOf3  = NumberOfMultiplesUpto(n, 3);
        const auto kTotalNoOfMultiplesOf5  = NumberOfMultiplesUpto(n, 5);
        const auto kTotalNoOfMultiplesOf15 = NumberOfMultiplesUpto(n, 15);
        
        const auto kSumOfMultiplesOf3      = SumOfFirstNaturalNumbers(kTotalNoOfMultiplesOf3) * 3;
        const auto kSumOfMultiplesOf5      = SumOfFirstNaturalNumbers(kTotalNoOfMultiplesOf5) * 5; 
        const auto kSumOfMultiplesOf15     = SumOfFirstNaturalNumbers(kTotalNoOfMultiplesOf15) * 15; 
        
        return kSumOfMultiplesOf3 + kSumOfMultiplesOf5 - kSumOfMultiplesOf15;
    }
};

auto Soln(unsigned int limit)
{
    unsigned long long sum = 0;
    for (unsigned int i = 3; i < limit; ++i)
    {
        if (i % 3 == 0 || i % 5 == 0) { sum += i; }
    }
    return sum;
}

int main()
{
    cout << "Please enter a number: ";
    auto n = size_t{ 0 };
    cin >> n;

    auto start_timepoint = steady_clock::now();
    auto sum             = SumOfNaturalNumbers_if_n(n, [](const auto &a){ return (a % 3 == 0) || (a % 5 == 0); }); 
    auto duration1       = steady_clock::now() - start_timepoint; 

    start_timepoint = steady_clock::now();
    auto sum2       = SumOfNaturalNumbersDivisileBy3And5{}(n);
    auto duration2  = steady_clock::now() - start_timepoint;

    start_timepoint = steady_clock::now();
    auto sum3       = Soln(n);
    auto duration3  = steady_clock::now() - start_timepoint;

    assert(sum == sum2 && sum2 == sum3);
    cout << "Sum of natural numbers divisilbe by 3 and 5 upto " << n << " is(using modulus)          : " << sum  << ", took: " << duration1.count() << "ns" << endl;
    cout << "Sum of natural numbers divisilbe by 3 and 5 upto " << n << " is(using sequence formulas): " << sum2 << ", took: " << duration2.count() << "ns" << endl;
    cout << "Sum of natural numbers divisilbe by 3 and 5 upto " << n << " is(book soln.)             : " << sum3 << ", took: " << duration3.count() << "ns";


    return 0;
}