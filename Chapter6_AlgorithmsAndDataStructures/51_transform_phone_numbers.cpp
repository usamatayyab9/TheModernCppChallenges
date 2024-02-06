/**
 * @file 51_transform_phone_numbers.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command: g++ -std=c++17 51_transform_phone_numbers.cpp
 *  This file is solution to "Problem 51. Transforming a list of phone numbers"
 *  mentioned in "Chapter 6: Algorithms and Data Structures" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Solution:
 * The function `TransformPhoneNumbers` implements the required solution.
 * Driver code:
 * - Initializes the a list of numbers
 * - Transform the phone numbers
 * - Prints all the transformed numbers
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

using std::begin;
using std::cbegin;
using std::cend;
using std::cout;
using std::copy;
using std::end;
using std::endl;
using std::ostream_iterator;
using std::remove;
using std::string;
using std::vector;

/**
 * @brief Transforms a vector of phone numbers to a standardized format.
 *
 * @details
 * - Removes white spaces from each phone number.
 * - Adds the specified country code with a '+' if the number starts with '0'.
 * - Adds a '+' to the beginning if the number starts with the specified country code.
 * - Adds the specified country code with a '+' if the number contains the country code
 *   but does not start with it.
 *
 * @param numbers_list A vector of phone numbers to be transformed.
 * @param country_code The country code to be added or checked in the phone numbers.
 */

void TransformPhoneNumbers(vector<string> &numbers_list, const string &country_code)
{
    const auto kCounterCodeWithPlus = "+" + country_code;
    for (auto &number : numbers_list)
    {
        /*! Erase all white spaces */
        number.erase(remove(begin(number), end(number), ' '),
            end(number)
        );

        if ('0' == number.front()) {
            number.erase(begin(number));
            number.insert(0, kCounterCodeWithPlus);
        }
        else if (0 == number.find(kCounterCodeWithPlus)) {
            /*! Do nothing */
        }
        else if (0 == number.find(country_code)) {
            number.insert(0, 1, '+');
        }
        else  {
            number.insert(0, kCounterCodeWithPlus);
        }
    }
}

int main()
{
    auto numbers = vector<string> {
        "07555 123456",
        "07555123456",
        "+92 7555 123456",
        "   92 7555 123456",
        "7555 123456"
    };
    TransformPhoneNumbers(numbers, "92");
    copy(cbegin(numbers), cend(numbers), ostream_iterator<string>{cout , "\n"});
    
    return 0;
}