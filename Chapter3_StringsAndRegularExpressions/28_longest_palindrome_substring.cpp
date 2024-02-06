/**
 * @file 28_longest_palindrome_substring.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 28_longest_palindrome_substring.cpp
 * 
 *  This file is solution to "Problem 28. Longest palindromic substring"
 *  mentioned in "Chapter 3: Strings and Regular Expressions" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a function that, given an input string, locates and returns the longest sequence in the
 * string that is a palindrome. If multiple palindromes of the same length exist, the first one
 * should be returned.
 * 
 * Solution:
 * - See function comments of LongestPalindromeSubstring() for understanding the algorithm
 * Driver code:
 * - Takes a string input from user
 * - Computes and prints the longest palidromic substring
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <iterator>
#include <unordered_map>
#include <utility>
#include <algorithm>

using std::cbegin;
using std::cend;
using std::cin;
using std::cout;
using std::copy;
using std::endl;
using std::equal;
using std::find_if;
using std::pair;
using std::string;
using std::string_view;
using std::unordered_map;
using std::vector;

/**
 * @brief   Get the Character Indices Map object
 * @param   str 
 * @return  unordered_map<char, vector<size_t>>
 *          For each character in str corresponding vector contains all
 *          indices at which it is present
 */
auto GetCharacterIndicesMap(string_view str)
{
    auto char_vec_umap = unordered_map<char, vector<size_t>>{};
    for (auto idx = size_t{ 0 }; idx < size(str) ;++idx)
    {
        const auto &kCurrentChar = str[idx];
        char_vec_umap[kCurrentChar].push_back(idx);
    }
    return char_vec_umap;
}

/**
 * @brief Tests whether a given string is palindrome or not
 * @param str - input string to test whether it is palindrome or not
 * @return true - If the @param str is palindrome
 * @return false - otherwise
 */
bool IsPalindrome(string_view str)
{
    return equal(cbegin(str), cbegin(str) + (size(str) / 2), crbegin(str));
}

/**
 * @brief Tests whether length of str is greater than @param len 
 *        and it is a palindrome or not. 
 * @param len 
 * @param str 
 * @return true, if str is palindrome and length of @param str > @param len
 * @return false, otherwise
 */
bool IsLongerPalindrome(size_t len, string_view str)
{
    return (size(str) > len) && (IsPalindrome(str));
}

/**
 * @brief Calculates the longest palindrome substring of param str
 * @details Algorithm works as follows
 *          1. Generates a map such that for each character in str, all the indices
 *              at which it occurs are stored in vector. All indices are in sorted order.
 *          2. Start traversing str from left call it left_idx:
 *              2.1 Fetch its indices vector from map
 *              2.2 Traverse the indices vector in reverse order i.e. larger indices first,
 *                   it will always be greater the left index. call it right_idx
 *              2.2.1 if length of substring [left_indx, right_idx + 1) is greater max length of
 *                      result string AND substring [left_indx, right_idx + 1) is also palindrome,
 *                      then update max lenght = right_idx - left_indx + 1. If condition is met
 *                      then stop traversing indices vector.
 * @param str 
 * @return string 
 */
string LongestPalindromeSubstring(string_view str)
{
    const auto kIndicesUmap           = GetCharacterIndicesMap(str);
    auto first                        = size_t{ 0 };
    auto max_palindrome_length_so_far = size_t{ 1 };

    for (auto left_idx = size_t{ 0 }; left_idx < size(str) ;++left_idx)
    {
        const auto &kCurrentCh  = str[left_idx];
        const auto &kIndicesVec = kIndicesUmap.at(kCurrentCh);
        if (size(kIndicesVec) > 1)
        {
            auto pos = find_if(crbegin(kIndicesVec), crend(kIndicesVec),
            [max_palindrome_length_so_far, left_idx, str](const auto &right_idx)
            {
                return IsLongerPalindrome(max_palindrome_length_so_far, str.substr(left_idx, right_idx - left_idx + 1));
            });
            if (pos != crend(kIndicesVec))
            {
                first                        = left_idx;
                const auto &kRightIdx        = *pos;
                max_palindrome_length_so_far = kRightIdx - left_idx + 1;
            }
        }
    }
    return string{str, first, max_palindrome_length_so_far};
}

int main()
{
    cout << "Enter string: ";
    auto str = string{};
    cin >> str;

    auto longest_palindrome_string = LongestPalindromeSubstring(str);
    cout << "Longest palindrome substring: " << longest_palindrome_string << '\n';
    return 0;
}