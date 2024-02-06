/**
 * @file 23_binary_to_string.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 *  Compilation command : g++ -std=c++17 23_binary_to_string.cpp
 *  This file is solution to "Problem 23.  Binary to string conversion"
 *  mentioned in "Chapter 3: Strings and Regular Expressions" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 *  Problem statement:
 *  "
 *   Write a function that, given a range of 8-bit integers (such as an array or vector), returns a
 *   string that contains a hexadecimal representation of the input data. The function should be
 *   able to produce both uppercase and lowercase content. Here are some input and output
 *   examples:
 *   Input: { 0xBA, 0xAD, 0xF0, 0x0D }, output: "BAADF00D" or "baadf00d"
 *   Input: { 1,2,3,4,5,6 }, output: "010203040506"
 *  "
 * This file implements two function for solution:
 * 1. BytesToString() which takes a container and bool flag for uppercase as input and returns a string.
 * 2. BytesToStringFold() which takes a bool flag for determining whether output characters should be in
 *      uppercase or lowercase and Bytes which has to be converted to string.
 * 
 * Driver code:
 * The program first initializes two different containers of different bytes, converts them to string
 * in lowercase and uppercase, prints the string on console.
 * Secondly, uses BytesToStringFold() funtion for converting diffetent characters to string.
 * 
 * @copyright Copyright (c) 2023
 */
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <sstream>
#include <iomanip>
#include <type_traits>

using std::array;
using std::cin;
using std::cout;
using std::endl;
using std::byte;
using std::ostream;
using std::setfill;
using std::setw;
using std::string;
using std::stringstream;
using std::vector;

/**
 * @brief converts elements of container into string such that each element is a byte
 * 
 * @tparam Container 
 * @param c 
 * @param is_uppercase 
 * @return string 
 */
template <class Container>
string BytesToString(const Container &c, const bool &is_uppercase)
{
    auto ss = stringstream{};
    if (is_uppercase) { ss.setf(std::ios_base::uppercase); }
    for (const auto &value : c)
    {
        ss << setw(2) << setfill('0') << std::hex << static_cast<int>(value);
    }
    return ss.str();
}

/**
 * @brief converst @param args into string
 * 
 * @tparam Bytes 
 * @param is_uppercase 
 * @param args 
 * @return string 
 */
template <typename ...Bytes>
string BytesToStringFold(const bool &is_uppercase, Bytes... args)
{
    auto sstream = stringstream{};
    if (is_uppercase) { sstream.setf(std::ios_base::uppercase); }
    (sstream << setw(2) << setfill('0') << std::hex  << static_cast<int>(args), ...);
    return sstream.str();
}

int main()
{
    constexpr auto kByteArray = array<unsigned char, 4>{ 1, 2, 3, 4 };
    const auto kByteVector    = vector<unsigned char> { 0xBA, 0xAD, 0xF0, 0x0D };
    
    cout << BytesToString(kByteArray, false) << '\n';
    cout << BytesToString(kByteArray, true) << '\n';
    cout << BytesToString(kByteVector, false) << '\n';
    cout << BytesToString(kByteVector, true) << '\n';

    cout << BytesToStringFold(true, byte{ 0xBA }, byte{ 0xAD }, byte{ 0xF0 }, byte{ 0x0D }) << '\n';
    cout << BytesToStringFold(false, byte{ 0xBA }, byte{ 0xAD }, byte{ 0xF0 }, byte{ 0x0D }) << '\n';
    
    return 0;
}