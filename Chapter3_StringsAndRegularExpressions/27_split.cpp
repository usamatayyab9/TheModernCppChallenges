/**
 * @file 27_split.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 *  Compilation command : g++ -std=c++17 27_split.cpp
 *  This file is solution to "Problem 27. Splitting a string into tokens with a list of possible delimiters"
 *  mentioned in "Chapter 3: Strings and Regular Expressions" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a function that, given a string and a list of possible delimiter characters, splits the
 * string into tokens separated by any of the delimiters and returns them in an std::vector.
 * Example: input: "this,is.a sample!!" with delimiters ",.! ", output: {"this", "is", "a", "sample"}.
 * 
 * Solution:
 * This provides only a single function called Split which takes a string and delimeters as input
 * and returns a vector of all possible tokens.
 * 
 * Driver code:
 * - Input 2 strings from user for representing target string and other representing a list of delimeters.
 * - Genrates token using Split function
 * - Prints all tokens on console 
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include <iterator>
#include <locale>
#include <cassert>
#include <vector>

using std::cbegin;
using std::cend;
using std::cin;
using std::cout;
using std::copy;
using std::find_first_of;
using std::ostream_iterator;
using std::string;
using std::string_view;
using std::vector;

/**
 * @brief Splits a string into tokens using specified delimiters.
 * 
 * This function splits the input string into tokens using the specified delimiters.
 * Tokens are stored in a vector and returned.
 * 
 * @param text - The string to be split into tokens.
 * @param delimiters - The set of characters considered as delimiters for tokenization.
 * @return vector<string> - The vector of tokens obtained by splitting the input string.
 */
auto Split(string_view text, string_view delimeters)
{
    auto tokens = vector<string>{};
    auto first  = cbegin(text);
    while (true)
    {
        auto last = find_first_of(first, cend(text), cbegin(delimeters), cend(delimeters));
        /*! [first, last) is a token or it can be an empty string
            It might be possible delimeters are consecutive, then
            first == last and token will be empty.
        */
        if (first != last) { tokens.push_back(string{ first, last }); }
        /*! If search is at end i.e last == end(text) then last + 1 will be going beyond
            the limit, therfore break the loop before. However if last != cend(text) then it
            means that there are some still characters left to process.
        */
        if (last == cend(text)) { break; }
        
        first = last + 1;
    }

    return tokens;
}

int main()
{
    auto text       = string{};
    auto delimeters = string{};
    
    cout << "Enter a string you want to split: ";
    getline(cin, text);

    cout << "Enter the list of delimeters: ";
    getline(cin, delimeters);

    const auto kTokens = Split(text, delimeters);
    cout << "List of tokens:\n";
    copy(cbegin(kTokens), cend(kTokens), ostream_iterator<string>{cout, "\n"});

    return 0;
}