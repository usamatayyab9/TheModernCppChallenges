/**
 * @file 25_capitalize.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 *  Compilation command : g++ -std=c++17 25_capitalize.cpp 
 *  This file is solution to "Problem 25. Capitalizing an article title"
 *  mentioned in "Chapter 3: Strings and Regular Expressions" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 *  Problem statement:
 *  Write a function that transforms an input text into a capitalized version, where every word
 *  starts with an uppercase letter and has all the other letters in lowercase. For instance, the
 *  text "the c++ challenger" should be transformed to "The C++ Challenger".
 * 
 * Solution:
 * A function Capitalize() which takes constant reference to string representing the article 
 * which is to be capitalized, converts the input to capitalized version and returns it.
 * 
 * Driver code:
 * Test cases for testing the above function.
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

using std::basic_string;
using std::basic_string_view;
using std::cbegin;
using std::cend;
using std::cin;
using std::cout;
using std::find;
using std::find_if_not;
using std::isalpha;
using std::locale;
using std::string;
using std::string_view;
using std::toupper;
using std::tolower;

/**
 * @brief Capitalizes the first letter of each word in the input string.
 * 
 * This function iterates through the input string, identifying words (sequences of alphabetic characters) 
 * and capitalizing the first letter of each word while converting the rest of the word to lowercase.
 * 
 * @tparam T - The character type of the string.
 * @param text - The input string to be capitalized.
 * @return basic_string<T> - A new string with the first letter of each word capitalized.
 */
template<class T>
auto Capitalize(const basic_string<T> &text)
{
    const auto loc  = locale{};
    auto IsAlpha    = [&loc](const auto &ch) { return isalpha(ch, loc); };
    auto result     = basic_string<T>{ text };
    auto first      = begin(result);
    while (true)
    {
        first = find_if(first, end(result), IsAlpha);
        if (first == end(result))
        {
            break;
        }
        auto last  = find_if_not(first, end(result), IsAlpha);
        /*! [first, last) represents a word*/
        *first = toupper(*first, loc);
        std::for_each(first + 1, last, [&loc](auto &ch) {ch = tolower(ch, loc); });
        first = last;
    }
    
    return result;
}

int main()
{
    using namespace std::string_literals;
    using namespace std::string_view_literals;
    
    assert("The C++ Challenge" == Capitalize("the c++ challenge"s));
    assert("The C++ Challenge" == Capitalize("The C++ Challenge"s));
    assert("Thec++Challenge" == Capitalize("thec++challenge"s));
    assert("The C++ Challenger" == Capitalize("the c++ challenger"s));
    assert("This Is An Example, Should Work!" == Capitalize("THIS IS an ExamplE, should wORk!"s));
    assert("++++Text----Inside=====Operators" == Capitalize("++++text----inside=====operators"s));
    assert("++++Text----Inside=====Operators" == Capitalize("++++text----inside=====operators"s));
    assert("     Empty Spaces Before Any Text" == Capitalize("     empty spaces before any text"s));
    assert("   A   Lot      Of   Spaces     Before  Between    And After Text.     " == Capitalize("   a   lot      of   spaces     before  between    and after text.     "s));

    cout << "All Test cases passed\n";
    return 0;
}