/**
 * @file 13_value_of_pi.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 *      Compilation command : g++ -std=c++17 13_value_of_pi.cpp
 *      This file is solution to "Problem 13. Computing the value of Pi"
 *      mentioned in "Chapter 1: Math Problems" of the book:
 *      - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *      
 *      The problem statement is:
 *      "Write a program that computes the value of Pi with a precision of two decimal digits."
 * 
 *      For calculating the value of Pi this 3 different approaches are implemneted:
 *      1. Using limit method
 *      2. Using Leibniz formula
 *      3. Chudnovsky algorithm for calculating Pi
 *      Driver code:
 *      The program calculates the value of Pi using all three algortihms above and prints them.
 *      See function  comments for each method implementation
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <cmath>
#include <limits>

using std::cout;
using std::endl;
using std::pow;

auto PIUsingLimitMethod()
{
    constexpr auto kMaxInt  = std::numeric_limits<double>::max();
    constexpr auto kAngle   = (180.0 / kMaxInt) * 0.0174533;
    const auto kSin         = sin(kAngle);
    const auto kPi          = kMaxInt * kSin;
    return kPi;
}

struct LeibnizFormulaForPI
{
    //https://en.wikipedia.org/wiki/Leibniz_formula_for_%CF%80
    double operator()(const size_t &N)
    {
        auto sum = 0.0;
        for (auto k = 0; k < N; ++k)
        {
            const auto kNumerator   = pow(-1, k);
            const auto kDenominator = (2.0 * k) + 1.0;
            sum += (kNumerator / kDenominator);
        }
        return sum * 4.0;
    }
};

struct ChudnovskyAlgorithmForPI
{
    /**
     * In 1989, the Chudnovsky brothers computed π to over 1 billion decimal places on the 
     * supercomputer IBM 3090 using the following variation of Ramanujan's infinite series.
     * For more details : https://en.wikipedia.org/wiki/Approximations_of_%CF%80#20th_and_21st_centuries 
    */
    long double operator()(const size_t &N)
    {
        using long_double = long double;
        constexpr auto kAddConstant         = long_double{ 13591409.0 };
        constexpr auto kMultiplierConstant  = long_double{ 54514013.0 };
        constexpr auto kBaseConstant        = long_double{ 640320.0   };
        constexpr auto kExponentAddConstant = long_double{ 3.0 / 2.0  };
        constexpr auto kNegativeOne         = long_double{ -1.0       };
        constexpr auto kPositiveOne         = long_double{ +1.0       };
        auto sum                            = long_double{ 0.0        };
        for (auto k = 0; k < N; ++k)
        {
            const auto kPowerOfNegativeOne  = (0 == k % 2) ? kPositiveOne : kNegativeOne;
            const auto kFactorialOf6k       = Factorial(6 * k);
            const auto kNumerator           = kPowerOfNegativeOne * kFactorialOf6k * (kAddConstant + kMultiplierConstant * k);
            const auto kFactorialOf3k       = Factorial(3 * k);
            const auto kCubeOfKFactorial    = pow(Factorial(k), 3);
            const auto kExponent            = (3.0 * k) + kExponentAddConstant;
            const auto kDenominator         = kFactorialOf3k * kCubeOfKFactorial * pow(kBaseConstant, kExponent);            
            sum += (kNumerator / kDenominator);
        }
        return 1.0 / (12.0 * sum);
    }

private:
    size_t Factorial(size_t n)
    {
        if (n <= 1)
        {
            return 1;
        }
        auto factorial = 1;
        for (; 1 != n; --n)
        {
            factorial *= n;
        }
        return factorial;
    }
};

int main()
{
    cout << "Value of π using limit method is: " << PIUsingLimitMethod() << '\n';

    auto number_of_terms_leibniz = 595;
    cout << "Value of π using Leibniz series upto " << number_of_terms_leibniz << " terms is: " << LeibnizFormulaForPI{}(number_of_terms_leibniz) << '\n';

    auto number_of_terms_chudnovsky = 3;
    cout << "Value of π using Chudnovksy algorithm upto " << number_of_terms_chudnovsky << " terms is: " << ChudnovskyAlgorithmForPI{}(number_of_terms_chudnovsky) << '\n';

    return 0;
}