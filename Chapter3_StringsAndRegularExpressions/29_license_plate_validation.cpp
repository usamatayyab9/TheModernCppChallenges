/**
 * @file 29_license_plate_validation.cpp
 * @author Usama Tayyab (usamtayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 29_license_plate_validation.cpp
 * This file is solution to "Problem 29. License plate validation"
 *  mentioned in "Chapter 3: Strings and Regular Expressions" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Considering license plates with the format LLL-LL DDD or LLL-LL DDDD (where L is an
 * uppercase letter from A to Z and D is a digit), write:
 *  - One function that validates that a license plate number is of the correct format
 *  - One function that, given an input text, extracts and returns all the license plate
 *      numbers found in the text
 * 
 * Solution:
 * To solve this problem regex approach is used. The regex for matching license plate is 
 * defined by constant variable @ref kLicensePlateRegex.
 * - For part 1 of the solution the function IsValidLicensePlateNumber() validates whether
 *      input string represent a valid license plate or not.
 * - For part 2 of the solution the function ExtractLicensePlateNumbers() which takes a string
 *      as an input representing the text and returns the list of all the valid license plates.
 * 
 * Driver code:
 * The program first takes a string input from user, then tests whether the given string is
 * a valid license plate or not using the function IsValidLicensePlateNumber() and prints the
 * result of the test on console.
 * 
 * Secondly program reads the file license_validation_text.txt into a string. Extracts
 * and prints all the license plate numbers found in the string. All license number are
 * extracted using the function ExtractLicensePlateNumbers().
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <string>
#include <string_view>
#include <regex>
#include <vector>
#include <fstream>
#include <iterator>
#include <cassert>

using std::back_inserter;
using std::cbegin;
using std::cend;
using std::cin;
using std::cout;
using std::copy;
using std::endl;
using std::ifstream;
using std::istreambuf_iterator;
using std::ostream_iterator;
using std::regex;
using std::regex_match;
using std::sregex_iterator;
using std::string;
using std::string_view;
using std::transform;
using std::vector;

const auto kLicensePlateRegex = regex{ "[A-Z]{3}-[A-Z]{2} [0-9]{3,4}" };

/**
 * @brief Tests whether the param to_match is validated by regex kLicensePlateRegex.
 * 
 * @param to_match 
 * @return true - if the @param to_match is in valid license plate format
 * @return false - otherwise
 */
bool IsValidLicensePlateNumber(const string &to_match)
{
    return regex_match(to_match, kLicensePlateRegex);
}

/**
 * @brief Extracts all strings from param text which are validated by regex kLicensePlateRegex.
 * 
 * @param text - Sequence of characters from which license plates are to be extracted
 * @return vector<string> - List of strings, such that each string is in valid license format. 
 */
vector<string> ExtractLicensePlateNumbers(const string &text)
{
    auto first = sregex_iterator{ cbegin(text), cend(text), kLicensePlateRegex };
    auto last  = sregex_iterator{};
    auto vec   = vector<string>{};
    transform(first, last, back_inserter(vec), [](const auto regex_match_info) {
        return regex_match_info.str();
    });

    return vec;
}

int main()
{
    auto str = string{};
    cout << "Enter a string to validate it as License plate number: ";
    getline(cin, str);
    const auto kValidationStatus = IsValidLicensePlateNumber(str);
    cout << "Validation status : " << std::boolalpha << kValidationStatus << '\n';

    auto fin                        = ifstream{ "license_validation_text.txt" };
    const auto kText                = string{ istreambuf_iterator<char>{ fin }, istreambuf_iterator<char>{} }; 
    const auto kLicensePlateNumbers = ExtractLicensePlateNumbers(kText);
    cout << "License plate numbers found in license_validation_text.txt:\n";
    copy(cbegin(kLicensePlateNumbers), cend(kLicensePlateNumbers), ostream_iterator<string>{ cout, "\n" });
    return 0;
}