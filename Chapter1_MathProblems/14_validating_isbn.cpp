/**
 * @file 14_validating_isbn.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 *      Compilation command: g++ -std=c++17 14_validating_isbn.cpp
 *      This file is solution to "Problem 14. Validating ISBNs"
 *      mentioned in "Chapter 1: Math Problems" of the book:
 *      - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *      
 *      The problem statement is:
 *      "Write a program that validates that 10-digit values entered by the user, as a string,
 *      represent valid ISBN-10 numbers."
 *      
 *      For ISBN:
 *      The International Standard Book Number (ISBN) is a numeric commercial book identifier
 *      that is intended to be unique. Publishers purchase or receive ISBNs from an affiliate
 *      of the International ISBN Agency.
 *      For more infor visit https://en.wikipedia.org/wiki/ISBN#Check_digits
 *      
 *      This file provides functions verifying both ISBN-10 and ISBN-13 numbers.
 *      Driver code:
 *      The program first takes an s=input from the user. Then ISBN-10 and ISBN-13 validation statuses of 
 *      input number;
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include <cctype>

using std::all_of;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::string_view;

/**
 * @brief Tests if @param ch is a digit or not
 * 
 * @param ch 
 * @return true - if ch is a digit
 * @return false - otherwise
 */
bool IsDigit(const char &ch)
{
    return ch >= '0' && ch <= '9';
}

/**
 * @brief Tests whether @param isbn fulfills the valid ISBN criteria
 *      A valid ISBN number would contain all digits and its size will
 *      be equal to @param expectec_size. Genrally an ISBN number is of length
 *      10 and 13. 
 * @param isbn 
 * @param expected_size 
 * @return true 
 * @return false 
 */
bool TestISBNSizeAndDigitsConstraint(string_view isbn, const size_t &expected_size)
{
    return (expected_size == size(isbn)) && all_of(cbegin(isbn), cend(isbn), IsDigit);
}

/**
 * @brief Tests if @param isbn is a valid ISBN-10 number
 *      Firsts verifies that param isbn of length 10 and all of its characters are digits.
 *      The calcule the sum of digits multiplied by their weight.
 *          Weight are assigned in decreasing from 10 to 1 i.e.
 *           - first digit in param isbn will assigned weight 10
 *           - second digit in param isbn will assigned weight 9
 *           - tenth digit in param isbn will assigned weight 1
 *      After calculating weighted sum, if sum modulus 11 is 0 then it is considered a valid ISBN
 *      number.
 * @param isbn 
 * @return true - if isbn is valid ISBN-10 number
 * @return false - false
 */
bool IsValidISBN_10(string_view isbn)
{
    auto is_valid = false;
    if (TestISBNSizeAndDigitsConstraint(isbn, 10))
    {
        auto weight = 10;
        auto sum    = 0;
        for (const auto &digit : isbn)
        {
            sum += weight * (digit - '0');
            --weight;
        }
        is_valid = (0 == (sum % 11));
    }
    return is_valid;
}

/**
 * @brief Tests if @param isbn is a valid ISBN-13 number
 *      Firsts verifies that param isbn of length 13 and all of its characters are digits.
 *      The calcule the sum of digits multiplied by their weight.
 *          Weight are assigned in alternating between 1 and 3 to alternate digits i.e.
 *              - digits at even index are assigned weight 1
 *              - digits at odd indices are assigned weight 3
 *      After calculating weighted sum, if sum modulus 10 is 0 then it is considered a valid ISBN-13
 *      number.
 * @param isbn 
 * @return true - if isbn is valid ISBN-10 number
 * @return false - false
 */
bool IsValidISBN_13(string_view isbn)
{
    auto is_valid = false;
    if (TestISBNSizeAndDigitsConstraint(isbn, 13))
    {
        auto weight = 1;
        auto sum    = 0;
        for (const auto &digit : isbn)
        {
            sum += weight * (digit - '0');
            const auto toggler = (1 == weight);
            weight = (3 * toggler) + ((1 - toggler) * 1);

            // if (1 == weight) { weight = 3; }
            // else             { weight = 1; }
        }
        is_valid = (0 == sum % 10);
    }
    return is_valid;
}

int main()
{
    auto str = string{};
    cout << "Enter ISBN to validate: ";
    cin >> str;

    cout << "ISBN-10 validation status : " << std::boolalpha << IsValidISBN_10(str) << '\n';
    cout << "ISBN-13 validation status : " << std::boolalpha << IsValidISBN_13(str) << '\n';
    return 0;
}