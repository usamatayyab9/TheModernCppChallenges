/**
 * @file 10_gray_code.cpp
 * @author Usam Tayyab (usamtayyab9@gmail.com)
 * @brief 
 *      Compilation command: g++ -std=c++17 9_primes_factors.cpp
 *      This file is solution to "Problem 10.  Gray Code"
 *      mentioned in "Chapter 1: Math Problems" of the book:
 *      - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *      
 *      The problem statement is:
 *      "Write a program that displays the normal binary representations, Gray code
 *      representations, and decoded Gray code values for all 5-bit numbers."
 * 
 *      Gray codes are defined as :
 *      The reflected binary code (RBC), also known as reflected binary (RB) or Gray code
 *      after Frank Gray, is an ordering of the binary numeral system such that two successive
 *      values differ in only one bit (binary digit). For example, the representation of
 *      the decimal value "1" in binary would normally be "001" and "2" would be "010".
 *      In Gray code, these values are represented as "001" and "011". That way, incrementing
 *      a value from 1 to 2 requires only one bit to change, instead of two. 
 *      For more info, visit https://en.wikipedia.org/wiki/Gray_code
 * 
 *      This uses three functions to implement the solution.
 *      1. ToBinary() which converts a number into binary string.
 *      2. BinaryToGray() which converts a binary string to gray code(also a binary string).
 *      3. GrayToBinary() which converts a gray code to binary string. 
 *      
 *      Driver code:
 *      The program start by initializing two variables indicating number of bits and
 *      a 'limit' upto which gray code will be printed. Program prints the following
 *      in tabular form:
 *          - Number in decimal form
 *          - Binary of number 
 *          - Gray code encoding of number
 *          - Decoded gray code
 *      dirver code prints the above info for all numbers from 0 to 'limit' constant defined.
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <iomanip>

using std::begin;
using std::cin;
using std::cout;
using std::endl;
using std::end;
using std::reverse;
using std::string;
using std::transform;

/**
 * @brief Converts a decimal number to its binary representation with a specified number of digits.
 *      
 *      The function iteratively divides the input decimal number N_arg by 2 and appends the remainder
 *      to a string until N is no longer greater than zero. The string is then reversed to obtain
 *      the binary representation. If the length of the binary string is less than the specified number
 *      of digits, it is left-padded with zeros.
 * 
 * @param N_arg - decimal number to be converted to binary
 * @param digits - Desired number of bits in binary representation of argument
 * @return string, binary representation of N_arg with specified number of bits 
 */
string ToBinary(const size_t &N_arg, const size_t &digits)
{
    auto str = string{};
    for (auto N = N_arg; 0 != N ;N /= 2)
    {
        str.push_back('0' + (N % 2));
    }
    if (size(str) < digits) { str.resize(digits, '0'); }
    reverse(begin(str), end(str));
    return str;
}

/**
 * @brief Converts a binary number to its Gray code representation.
 * 
 *      The function creates a string to store the Gray code equivalent of the input binary string.
 *      It initializes the first bit of the Gray code with the same value as the first bit of the input binary.
 *      It then uses the exclusive OR (XOR) operation between consecutive bits of the binary string
 *      to calculate the corresponding Gray code bit.
 * 
 * @param bin - binary which has to be converted to gray code
 * @return string - a binary string in which param bin is expressed as gray code 
 */
string BinaryToGray(const string &bin)
{
    auto gray = string(size(bin), '0');
    gray.front() = bin.front();
    transform(cbegin(bin) + 1, cend(bin), cbegin(bin), begin(gray) + 1,
    [](const auto &prev, const auto &next)
    {
        const auto kPrevInt = prev - '0';
        const auto kNextInt = next - '0';
        const auto kGrayBit = (kNextInt + kPrevInt) % 2;
        return kGrayBit + '0';
    });
    return gray;
}

/**
 * @brief Converts a Gray code number to its binary representation.
 * 
 *      The function creates a string to store the binary equivalent of the input Gray code string.
 *      It initializes the first bit of the binary with the same value as the first bit of the input Gray code.
 *      It then uses the exclusive OR (XOR) operation between consecutive bits of the Gray code string
 *      and the corresponding bits of the previously calculated binary string to reconstruct the original binary.
 * 
 * @param gray - The Gray code number to be converted to binary.
 * @return A string representing the binary equivalent of the input Gray code.
 */
string GrayToBinary(const string &gray)
{
    auto binary = string(size(gray), '0');
    binary.front() = gray.front();
    transform(cbegin(gray) + 1, cend(gray), cbegin(binary), begin(binary) + 1,
    [](const auto &gray_ch, const auto &prev_binary_ch)
    {
        const auto kBinaryInt      = prev_binary_ch - '0';
        const auto kGrayInt        = gray_ch        - '0';
        const auto kCurrentGrayBit = (kBinaryInt + kGrayInt) % 2;
        return kCurrentGrayBit + '0';
    });
    return binary;
}

int main()
{
    constexpr auto kNumberOfBits = 5;
    constexpr auto kLimit        = 0b11'111;

    cout << "|‾‾‾‾‾‾‾‾|‾‾‾‾‾‾‾‾|‾‾‾‾‾‾‾‾‾‾‾‾‾‾|‾‾‾‾‾‾‾‾‾‾‾‾‾‾|\n";
    cout << "| Number | Binary | Gray Encoded | Gray Decoded |\n";
    cout << "|--------|--------|--------------|--------------|\n";

    for (auto i = 0; i <= kLimit ;++i)
    {
        const auto kBin     = ToBinary(i, kNumberOfBits);
        const auto kGrayEnc = BinaryToGray(kBin);
        const auto kGrayDec = GrayToBinary(kGrayEnc);
        cout << "| " << std::setw(6) << i            << " |  " 
             << kBin                                 << " | " 
             << std::setw(12) << kGrayEnc << " |  "
             << std::setw(11) << kGrayDec << " | "
             << '\n';
        cout << "|________|________|______________|______________|\n";
    }
    return 0;
}