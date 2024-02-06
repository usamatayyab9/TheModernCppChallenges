/**
 * @file 12_largest_collatz_sequence.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 *      Compilation command : g++ -std=c++17 12_largest_collatz_sequence.cpp
 *      This file is solution to "Problem 12. Largest Collatz sequence"
 *      mentioned in "Chapter 1: Math Problems" of the book:
 *      - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *      
 *      The problem statement is:
 *      "Write a program that determines and prints which number up to 1 million produces the 
 *      longest Collatz sequence and what its length is." 
 *      
 *      Explanation about Collatz conjecture:
 *      The Collatz conjecture is one of the most famous unsolved problems in mathematics.
 *      The conjecture asks whether repeating two simple arithmetic operations will
 *      eventually transform every positive integer into 1.
 *      It is named after the mathematician Lothar Collatz, who introduced the idea in
 *      1937, two years after receiving his doctorate. It is also known as the 3n + 1
 *      problem (or conjecture).
 *      Consider the following operation on an arbitrary positive integer(n):
 *          - If the number is even, divide it by two i.e. n/2.
 *          - If the number is odd, triple it and add one i.e. 3n + 1.
 *          - Repeat until n is not 1
 *      For instance, starting with n = 12 and applying the above steps without "shortcut",
 *      one gets the sequence 12, 6, 3, 10, 5, 16, 8, 4, 2, 1.
 *      
 *      The Collatz conjecture is: This process will eventually reach the number 1,
 *      regardless of which positive integer is chosen initially.
 *      For more visit https://en.wikipedia.org/wiki/Collatz_conjecture
 * 
 *      This solution uses a struct CollatzSequence for calculating number of steps of collatz sequence.
 *      Driver code: 
 *      Program starts with 1 and goes upto a specified constant value of 1,000,000 and calculates 
 *      length of collatz sequence length of each number. While doing so it keeps track of maximum
 *      sequence length obtained so far and which number it is obtained. Finally prints number and 
 *      sequence length which is maximum.
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <unordered_map>

using std::cout;
using std::cin;
using std::endl;
using std::unordered_map;


/**
 * @brief A structure for computing the Collatz sequence length for a given number.
 * 
 * The Collatz sequence is defined as follows:
 * - Start with any positive integer N.
 * - Each subsequent term is obtained from the previous term as follows:
 *   - If the previous term is even, the next term is one-half of the previous term.
 *   - If the previous term is odd, the next term is 3 times the previous term plus 1.
 * - The sequence ends when it reaches 1.
 * 
 * The structure uses memoization to cache previously computed sequence lengths to optimize
 * the computation for repeated values.
 */
struct CollatzSequence
{
    size_t operator()(const size_t &N_arg)
    {
        return Utility(N_arg);
    }

    private:
    unordered_map<size_t, size_t> lookup_table{ {0, 0} };
    
    bool IsAlreadyComputed(const size_t &N)
    {
        return lookup_table.count(N) == 1;
    }

    size_t Utility(const size_t &N_arg)
    {
        if (false == IsAlreadyComputed(N_arg))/*If value is cached then dont compute again.*/
        {
            if (1 == N_arg) { lookup_table[N_arg] = 1; } /*Base case*/
            else
            {
                /*General case*/
                if (0 == (N_arg % 2)) { lookup_table[N_arg] = 1 + Utility(N_arg / 2);     }
                else                  { lookup_table[N_arg] = 1 + Utility(3 * N_arg + 1); }
            }
        }
        return lookup_table[N_arg];
    }
};

int main()
{
    constexpr auto kLimit           = 1'000'000;
    auto collatz_sequence_generator = CollatzSequence{};
    
    cout << "Computing largest collatz sequence upto " << kLimit << "...\n";
    
    auto largest_collatz_length = 1;
    auto largest_collatz_number = 1;
    for (auto i = 1; i <= kLimit ;++i)
    {
        if (auto length = collatz_sequence_generator(i); length > largest_collatz_length)
        {
            largest_collatz_number = i;
            largest_collatz_length = length;
        }
    }
    cout << "Largest collatz sequence length:  " << largest_collatz_length << '\n';
    cout << "Largest collatz sequence number:  " << largest_collatz_number << '\n';
    return 0;
}