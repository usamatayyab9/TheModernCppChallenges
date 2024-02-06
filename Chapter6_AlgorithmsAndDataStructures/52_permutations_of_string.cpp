/**
 * @file 52_permutations_of_string.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 52_permutations_of_string.cpp
 * 
 * This file is solution to "Problem 52. Generating all the permutations of a string"
 * mentioned in "Chapter 6: Algorithms and Data Structures" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Solution:
 * The struct `PermutationPrinter` implements the required solution. Two member functions
 * `PrintIterative` and `PrintRecursive` are provided for prints all permutations of
 * given string on stream. See function comments for details.
 * 
 * Driver code:
 * Constructs an  of `PermutationPrinter`.
 * Input a string from user via console.
 * Then uses `PrintIterative` and `PrintRecursive` to print all
 * permutations of string.
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <string>
#include <vector>
#include <string_view>
#include <algorithm>
#include <iterator>
#include <stack>

using std::cbegin;
using std::cend;
using std::cin;
using std::cout;
using std::endl;
using std::find;
using std::ostream;
using std::ostream_iterator;
using std::size;
using std::stack;
using std::string;
using std::string_view;
using std::transform;
using std::vector;

/**
 * @brief Struct for printing permutations of a string.
 * 
 * @details
 * The `PermutationPrinter` struct provides methods to print all permutations
 * of a given string, both recursively and iteratively.
 */
struct PermutationPrinter
{
    /**
     * @brief Prints a string based on a vector of indices.
     * 
     * @param str The original string.
     * @param indices The vector of indices representing a permutation.
     * @param out The output stream to print the permutation.
     */
    void PrintStringFromIndicesVector(const string &str, const vector<size_t> &indices, ostream &out)
    {
        transform(cbegin(indices), cend(indices), ostream_iterator<char>{ out },
        [&str](const auto &idx) {
            return str[idx];
        });
    }

    /**
     * @brief Utility function for recursive permutation printing.
     * 
     * @param str The original string.
     * @param indices The current vector of indices.
     * @param out The output stream to print the permutations.
     * @param delim The delimiter between permutations.
     */
    void UtilityRec(const string &str, vector<size_t> &indices, ostream &out, string_view delim)
    {
        if (size(str) == size(indices))
        {
            PrintStringFromIndicesVector(str, indices, out);
            out << delim;
        }
        else
        {
            for (auto idx = size_t{ 0 }; idx < size(str) ;++idx)
            {
                if (find(cbegin(indices), cend(indices), idx) == cend(indices))
                {
                    indices.push_back(idx);
                    UtilityRec(str, indices, out, delim);
                    indices.pop_back();
                }
            }
        }
    }

    ostream& PrintRecursive(const string &str, ostream &out, string_view delim = "\n")
    {
        auto indices = vector<size_t>{};
        UtilityRec(str, indices, out, delim.data());
        return out;
    }

    /**
     * @brief Prints all permuation of @param str on stream object @param out.
     * Such that permutations are separated by @param delim.
     * 
     * Iterative approach uses a stack which stores indices which are considered
     * for printing.
     * 
     * @param str 
     * @param out 
     * @param delim 
     * @return ostream& 
     */
    ostream& PrintIterative(const string &str, ostream &out, string_view delim = "\n")
    {
        auto stk = stack<vector<size_t>>{};
        stk.push({});
        while (false == stk.empty())
        {
            auto indices = stk.top();
            stk.pop();
            if (size(str) == size(indices))
            {
                PrintStringFromIndicesVector(str, indices, out);
                out << delim;
            }
            else
            {
                for (auto idx = size_t{ 0 }; idx < size(str) ;++idx)
                {
                    if (find(cbegin(indices), cend(indices), idx) == cend(indices))
                    {
                        indices.push_back(idx);
                        stk.push(indices);
                        indices.pop_back();
                    }
                }
            }
        }
        return out;
    }
};


int main()
{
    auto permutation_logger= PermutationPrinter{};
    cout << "Enter string you want to permutations of: ";
    auto str = string{};
    cin >> str;
    permutation_logger.PrintRecursive(str, cout);
    cout << endl;

    permutation_logger.PrintIterative(str, cout);

    return 0;
}