/**
 * @file 11_numeral_to_roman.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 *      Compilation command : g++ -std=c++17 11_numeral_to_roman.cpp
 *      This file is solution to "Problem 11.  Converting numerical values to Roman"
 *      mentioned in "Chapter 1: Math Problems" of the book:
 *      - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *      
 *      The problem statement is:
 *      "Write a program that, given a number entered by the user, prints its Roman numeral equivalent"
 * 
 *      The struct NumeralToRomanConverter is used for converting a number into its Roman representation.
 *       Driver code:
 *      The program intially asks the user to input a number. The prints it roman eqibalent string.
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <array>

using std::array;
using std::cin;
using std::cout;
using std::endl;
using std::pair;
using std::string;
using std::string_view;

/**
 * @brief A structure for converting a number to its eqivalent roman literal.
 * 
 */
struct NumeralToRomanConverter
{
    string operator()(const size_t &num)
    {
        using namespace std::string_view_literals;
        constexpr auto kUnitPlaceTable     = array{ "I"sv, "II"sv, "III"sv, "IV"sv, "V"sv, "VI"sv, "VII"sv, "VIII"sv, "IX"sv };
        constexpr auto kTensPlaceTable     = array{ "X"sv,"XX"sv, "XXX"sv, "XL"sv, "L"sv, "LX"sv ,"LXX"sv, "LXXX"sv, "XC"sv  };
        constexpr auto kHunderedPlaceTable = array{ "C"sv, "CC"sv, "CCC"sv, "CD"sv, "D"sv, "DC"sv, "DCC"sv, "DCCC"sv, "CM"sv };
        constexpr auto kThousand           = 'M';
        
        auto value        = num;
        auto romans_array = array<string_view, 4>{}; // contain roman for each of unit, tens, hundered place of num

        if (const auto kUnitDigit = value % 10; kUnitDigit > 0) { romans_array[3] = kUnitPlaceTable[kUnitDigit - 1]; }
        value /= 10;

        if (const auto kTensDigit = value % 10; kTensDigit > 0) { romans_array[2] = kTensPlaceTable[kTensDigit - 1]; }
        value /= 10;

        if (const auto kHunderedDigit = value % 10; kHunderedDigit > 0) { romans_array[1] = kHunderedPlaceTable[kHunderedDigit - 1]; }
        value /= 10;

        if (value > 0) { romans_array[0] = string(value, kThousand); }
        /*!
         * At this point:
         * romans_array[0] will contain roman literal for value's thousand place digit 
         * romans_array[1] will contain roman literal for values's hundered place digit
         * romans_array[2] will contain roman literal for values's tens place digit
         * romans_array[3] will contain roman literal for values's unit place digit
         * Concatinating them together will yeild the roman for 'value'
         */

        const auto kRomanLiteralSize = size(romans_array[0]) + size(romans_array[1]) + size(romans_array[2]) + size(romans_array[3]);
        auto roman                   = string{};
        roman.reserve(kRomanLiteralSize);
        roman.append(romans_array[0]).append(romans_array[1]).append(romans_array[2]).append(romans_array[3]);
        return roman;
    }
};
int main()
{
    cout << "Enter a number: ";
    auto n = size_t{ 0 };
    cin >> n;
    cout << n << " converted to roman is " << NumeralToRomanConverter{}(n) << '\n';
    return 0;
}