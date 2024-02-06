/**
 * @file 31_transform_dates.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 31_transform_dates.cpp 
 * 
 * This file is solution to "Problem 31. Transforming dates in strings"
 *  mentioned in "Chapter 3: Strings and Regular Expressions" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a function that, given a text containing dates in the format dd.mm.yyyy or dd-mm-yyyy,
 * transforms the text so that it contains dates in the format yyyy-mm-dd.
 * 
 * 
 * Solution:
 * A function FormatDate() which takes a string as input parameter and returns the result of formatting.
 * The formattin results are specified by struct `DateFormatResult`.
 * See function comments for more details.
 * 
 * Driver code:
 * The program initializes multiple string and formats them using the function FormatDate(). For each of
 * the string if the formatting is successfull prints the formatted string otherwise prints an error message.
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <regex>
#include <string>
#include <string_view>

using std::cin;
using std::cout;
using std::endl;
using std::regex;
using std::regex_match;
using std::regex_replace;
using std::string;
using std::string_view;

/**
 * @brief 
 * A structure for denoting date formatting result.
 */
struct DateFormatResult
{
    bool    success;
    string  date_str;
};

/**
 * @brief   Converts the @param str into the format YYYY-MM-DD
 * 
 * @details @param str must be of the format DD, MM and YYYY separated by
 *          either dot(.) or dash(-). The input string is first validated
 *          as using the regex ((\d{2})(.|-)(\d{2})(.|-)(\d{4})). If the
 *          validation is successfull only then formatting is done.
 * @param   str 
 * @return  DateFormatResult : Contains two variables 'success' and 'date_str'
 *          If param str is a valid input then success = true and date_str = formatted string
 *          else success = false and date_str = empty string
 */
DateFormatResult FormatDate(string_view str)
{
    static const auto kDateRegex = regex{ R"((\d{2})(.|-)(\d{2})(.|-)(\d{4}))" };
    auto date_formatting_result  = DateFormatResult{ false, "" };
    if (const auto kIsValidInput = regex_match(str.data(), kDateRegex); kIsValidInput)
    {
        date_formatting_result.date_str = regex_replace(str.data(), kDateRegex, "$5-$3-$1");
        date_formatting_result.success  = true;
    }
    return date_formatting_result;
}

int main()
{
    auto str1 = string{ "18.03.1997" };
    auto str2 = string{ "18-03-1997" };
    auto str3 = string{ "AB-023-1997" };

    for (const auto &str : { str1, str2, str3 })
    {
        if (const auto [kStatus, kStr] = FormatDate(str); kStatus)
        {
            cout << str << " formatted in format(YYYY-MM-DD): " << kStr << '\n';
        }
        else
        {
            cout << "Unable to format date\n";    
        }
    }

    return 0;
}