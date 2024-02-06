/**
 * @file 50_filter_phone_numbers.cpp
 * @author your name (you@domain.com)
 * @brief 
 * Compilation command : g++ -std=c++17 50_filter_phone_numbers.cpp
 * 
 *  This file is solution to "Problem 50. Filtering a list of phone numbers"
 *  mentioned in "Chapter 6: Algorithms and Data Structures" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a function that, given a list of phone numbers,
 * returns only the numbers that are from a specified country.
 * The country is indicated by its phone country code,
 * such as 44 for Great Britain. Phone numbers may start with
 * the country code, a + followed by the country code, or have
 * no country code. The ones from this last category must be ignored.
 * 
 * Solution:
 * The solution is provided by function overloads `FilterNumbers`.
 * One overload takes two iterators and country code as input
 * Other overload takes templated continer and country code
 * as input.
 * Both function returns the list of all the number that start
 * with the provided country code.
 * 
 * Driver code:
 * - Initialize a vector of string with different phone numbers
 * - Passes the vector to `FilterNumbers`.
 * - Prints all the number returned by `FilterNumbers`.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <algorithm>
#include <iterator>

using std::cbegin;
using std::cend;
using std::cout;
using std::copy_if;
using std::endl;
using std::vector;
using std::string;
using std::string_view;
using std::back_inserter;

/**
 * @brief returns a list of all string that starts with a param country code
 * 
 * @tparam InputIterator - Type of iterator
 * @param first - pointing to the first element
 * @param last - pass the ed iterator
 * @param country_code - all number with this country code will be filtered
 * @return vector of all filtered numbers 
 */
template <class InputIterator>
auto FilterNumbers(InputIterator first, InputIterator last, string_view country_code)
{
    auto phone_numbers = vector<string>{};
    copy_if(first, last, back_inserter(phone_numbers), [&country_code](const string &str){
        auto pos = str.find(country_code);
        return (0 == pos) || ('+' == str[0] && 1 == pos);
    });
    return phone_numbers;
}

/**
 * @brief Overload that takes a container and delegates to the iterator-based version.
 */
template <class Container>
auto FilterNumbers(const Container &c, string_view country_code)
{
    return FilterNumbers(cbegin(c), cend(c), country_code);
}

int main()
{
    auto numbers = vector<string> {
        "+40744909080",
        "44 7520 112233",
        "+44 7555 123456",
        "40 7200 123456",
        "92 042 1234567",
        "7555 123456",
        "+92 333 1234567"
    };
    auto filtered = FilterNumbers(numbers, "92");
    for (auto const & number : filtered)
    {
        cout << number << endl;
    }

    return 0;
}