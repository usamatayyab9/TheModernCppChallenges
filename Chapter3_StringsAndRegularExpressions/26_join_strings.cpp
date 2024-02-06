/**
 * @file 26_join_strings.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 26_join_strings.cpp
 * This file is solution to "Problem 26. Joining strings together separated by a delimiter"
 *  mentioned in "Chapter 3: Strings and Regular Expressions" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a function that, given a list of strings and a delimiter, creates a new string by
 * concatenating all the input strings separated with the specified delimiter. The delimiter
 * must not appear after the last string, and when no input string is provided, the function
 * must return an empty string.
 * Example: input { "this","is","an","example" } and delimiter ' ' (space), output:
 * "this is an example".
 * 
 * Solution:
 * A function JoinStringsWithDelimeter() which takes container of strings and delimter and joins all the strings.
 * See function details for implementation details.
 * 
 * Driver code:
 * Executes the above function with different argumetns and compares the output is an assertion.
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cassert>
#include <list>
#include <array>
#include <forward_list>

using std::accumulate;
using std::array;
using std::cbegin;
using std::cend;
using std::copy;
using std::cout;
using std::endl;
using std::forward_list;
using std::list;
using std::next;
using std::size;
using std::string;
using std::string_view;
using std::vector;

/**
 * @brief Joins a collection of strings with a specified delimiter.
 * The algorithm works as follows:
 * - First calculetes the lenght of all strings added together and number of strings in
 *  container @param c.
 * - The calculates the size concatenated string
 * - Allocates the result string
 * - forevery string in c:
 *  - copy string into result string
 *  - Copy delimiter string into result string iff string is not the last string in param c.
 * @tparam Container - The type of the container holding the strings.
 * @param c - The container of strings to be joined.
 * @param delimiter - The string to be inserted between each pair of adjacent strings.
 * @return string - The concatenated string
 */
template<class Container>
string JoinStringsWithDelimeter(const Container &c, string_view delimeter)
{
    /*!
        Almost all container provide size() function which returns number of elements in container.
        std::forward_list does not provide any functionality for fetching number of elements
        in it. One method would be to write a separate size function using templates for std::forward_list
        and for other containers use their size(). So therefore for-loop is used to calculate size.
    */
    auto accumulated_string_lengths = size_t{ 0 };
    auto container_size             = size_t{ 0 };
    for (const auto &str : c)
    {
        accumulated_string_lengths += size(str);
        ++container_size;
    }
    const auto kTotalDelimeters = 0 == container_size ? 0 : size(delimeter) * (container_size - 1);
    const auto kTotalSize       = accumulated_string_lengths + kTotalDelimeters;
    auto str                    = string(kTotalSize, '\0');
    if (container_size > 0)
    {
        auto idx              = size_t{ 0 };
        auto destination_iter = begin(str);
        for (const auto &kStr : c)
        {
            destination_iter = copy(cbegin(kStr), cend(kStr), destination_iter);
            if (idx != container_size - 1)//For idx == last element dont add delimeter
            {
                destination_iter = copy(cbegin(delimeter), cend(delimeter), destination_iter);
            }
            ++idx;
        }
    }
    return str;
}

int main() 
{
    using namespace std::string_view_literals;
    using namespace std::string_literals;
    assert(JoinStringsWithDelimeter( vector{ "this"sv, "is"sv, "an"sv, "example"sv }, " ") == "this is an example"s);
    assert(JoinStringsWithDelimeter( vector{ "example"sv }                          , " ") == "example"s);
    assert(JoinStringsWithDelimeter( vector{ ""sv }                                 , " ") == ""s);
    assert(JoinStringsWithDelimeter( vector<string>{}                               , " ") == ""s);
    
    assert(JoinStringsWithDelimeter( list<string>{ "this", "is", "an", "example", "of", "list"}, " ") == "this is an example of list"s);
    assert(JoinStringsWithDelimeter( list{ "Hello"sv, "world"sv }                      , ";")          == "Hello;world"s);
    assert(JoinStringsWithDelimeter( list{ "Hello"sv, "world"sv, "with"sv, "multi"sv, "character"sv, "delimeter"sv} , ";;;") == "Hello;;;world;;;with;;;multi;;;character;;;delimeter"s);
    assert(JoinStringsWithDelimeter( list{ ""sv }                                 , "-") == ""s);
    assert(JoinStringsWithDelimeter( list{ ""sv, ""sv, ""sv, ""sv }               , "-") == "---"s);
    assert(JoinStringsWithDelimeter( list{ ""sv, ""sv, ""sv, ""sv }               , "") == ""s);
    assert(JoinStringsWithDelimeter( list{ ""sv }                                 , "") == ""s);
    assert(JoinStringsWithDelimeter( list<string>{}                               , "") == ""s);
    assert(JoinStringsWithDelimeter( list<string>{}                               , "+++") == ""s);

    assert(JoinStringsWithDelimeter( array{ "This"s, "is"s, "an"s, "array"s} , " ") == "This is an array"s);
    assert(JoinStringsWithDelimeter( array{ ""sv, ""sv, ""sv, ""sv} , "Four empty strings in container and I am delimeter.") == "Four empty strings in container and I am delimeter.Four empty strings in container and I am delimeter.Four empty strings in container and I am delimeter."s);
    assert(JoinStringsWithDelimeter( array{ ""sv, ""sv, ""sv, ""sv} , "Four empty strings in container and I am delimeter.") == "Four empty strings in container and I am delimeter.Four empty strings in container and I am delimeter.Four empty strings in container and I am delimeter."s);

    assert(JoinStringsWithDelimeter( forward_list<string_view>{ "I"sv, "Am"sv, "A"sv, "Forward"sv, "List"sv, "Without"sv, "Any"sv, "Delimeter"sv} , "") == "IAmAForwardListWithoutAnyDelimeter"s);

    cout << "All tests passed!!!\n";
    return 0;
}