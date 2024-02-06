/**
 * @file 24_string_to_binary.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 *  Compilation command : g++ -std=c++17 24_string_to_binary.cpp
 *  This file is solution to "Problem 24.   String to binary conversion"
 *  mentioned in "Chapter 3: Strings and Regular Expressions" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 *  Problem statement:
 *  Write a function that, given a string containing hexadecimal digits as the input argument,
 *  returns a vector of 8-bit integers that represent the numerical deserialization of the string
 *  content. The following are examples:
 *  Input: "BAADF00D" or "baadF00D", output: {0xBA, 0xAD, 0xF0, 0x0D}
 *  Input "010203040506", output: {1, 2, 3, 4, 5, 6}
 * 
 * This file provides three functions to implement the solution.
 * 1. HexToInt() which is used to convert two characters into a Hex value.
 * 2. ExtractAlphaNumericCharacters() which takes string_view as input returns a
 *      string which contains only alpha numeric characters from argument.
 * 3. StringToBinary() which takes a string as input and converts it to binary.
 * 
 * Driver code:
 * The program first initializes a string then converts it into binary and
 * prints the binary into required format.
 * 
 * @copyright Copyright (c) 2023
 */

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iomanip>

using std::back_inserter;
using std::cin;
using std::copy;
using std::copy_if;
using std::cout;
using std::endl;
using std::ostream_iterator;
using std::quoted;
using std::string;
using std::string_view;
using std::vector;

/**
 * @brief Converts @param ch1 and @param ch2 to Hex
 * 
 * @param ch1 
 * @param ch2 
 * @return unsigned char 
 */
unsigned char HexToInt(const char &ch1, const char &ch2)
{
    auto HexCode = [](const char &ch){
        if (ch >= '0' && ch <= '9') { return ch - '0';      }
        if (ch >= 'A' && ch <= 'Z') { return 10 + ch - 'A'; }
        if (ch >= 'a' && ch <= 'z') { return 10 + ch - 'a'; }
    };
    return HexCode(ch1) * 16 + HexCode(ch2);
}

/**
 * @brief Returns a string containing all alphanumeric characters from the input string @param str_input.
 * 
 * @param str_input - The input string to extract alphanumeric characters from.
 * @return string - A string containing only alphanumeric characters.
 */
string ExtractAlphaNumericCharacters(string_view str_input)
{
    auto str = string{};
    copy_if(cbegin(str_input), cend(str_input), back_inserter(str), [](const auto &ch) {
        return isalnum(ch);
    });
    return str;    
}

/**
 * @brief Converts a string of hexadecimal characters in @param str_input to binary representation.
 * 
 * - First extracts all alphanumeric characters from @param str_input.
 * - If the number of alphanumeric characters is odd, takes the first character and treats it as if
 *   the second character is '0'.
 * - Converts pairs of alphanumeric characters to their binary representation.
 * 
 * @param str_input - The input string containing hexadecimal characters.
 * @return vector<unsigned char> - The binary representation of the input string.
 */
vector<unsigned char> StringToBinary(string_view str_input)
{
    const auto kStr = ExtractAlphaNumericCharacters(str_input);
    auto vec        = vector<unsigned char>{};
    auto iter       = cbegin(kStr);
    
    if (1 == (size(kStr) % 2))
    {
        vec.push_back(HexToInt('0', kStr.front()));
        ++iter;
    }
    
    for (; iter < cend(kStr) ; iter += 2)
    {
        const auto &ch1 = *iter;
        const auto &ch2 = *(iter + 1); 
        vec.push_back(HexToInt(ch1, ch2));
    }

    return vec;
}

int main()
{
    auto str = string{ "BAADF00D" };
    auto vec = StringToBinary(str);
    cout << std::quoted(str) << " converted to bytes:\n";
    for (const auto &elem : vec)
    {
        cout << "0x" << std::hex << std::uppercase
        << std::setw(2) << std::setfill('0')
        << static_cast<int>(elem) << " ";
    }
    cout << '\n';
    return 0;
}