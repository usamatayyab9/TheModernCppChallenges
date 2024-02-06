/**
 * @file 88_caesar_cipher.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compile command : g++ -std=c++17 88_caesar_cipher.cpp
 * 
 *  This file is solution to "Problem 88. Caesar cipher"
 *  mentioned in "Chapter 11 : Cryptography" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution :
 * For details on caesar cipher visit : https://en.wikipedia.org/wiki/Caesar_cipher
 * Two functions for `Encrypt()` and `Decrypt()` are implemented. Both these functions
 * are defined in a namespace `CaesarCipher`. Both these function also allows the
 * caller to specify the shift amount as an argument.
 * See function comments for implmentation details.
 * 
 * Driver code:
 * - The program initalizes a string which contains all lowercase and uppercase
 * alphabets and some digits.
 * - The initalizes a loop from i = 1 to 26 inclusive.
 * - For every iteration of loop :
 *      - Encrypt the string using shift equal to i.
 *      - Decrypt the the encrypted string using shift equal to i.
 *      - Compares the decypted text with the original string.
 * - Above acts a testing script for encrypt and decrypt functions.
 * - If any test case fails the prints info about the failing tests.
 * - If all test cases pass successfully then not output is generated.   
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <string_view>
#include <string>
#include <algorithm>
#include <iterator>
#include <locale>

using std::begin;
using std::cbegin;
using std::cend;
using std::cout;
using std::isalpha;
using std::islower;
using std::locale;
using std::size;
using std::string;
using std::string_view;
using std::transform;

namespace CaesarCipher
{
    /**
    * @brief Encrypts the given text using a Caesar cipher with the specified shift.
    * 
    * @param text The text to be encrypted.
    * @param shift The shift value for the Caesar cipher.
    * @return std::string The encrypted text.
    */
    string Encrypt(string_view text, const int &shift)
    {
        auto str = string(size(text), '\0');
        transform(cbegin(text), cend(text), begin(str), [&shift](const auto &ch) -> char {
            auto transformed_ch = ch;
            if (const auto loc = locale{}; isalpha(ch, loc))
            {
                const auto kA          = islower(transformed_ch, loc) ? 'a' : 'A';
                const auto kAlphaIndex = static_cast<int>(ch - kA);
                transformed_ch         = kA + ((kAlphaIndex + shift) % 26);
            }
            return transformed_ch;
        });
        return str;
    }

    /**
     * @brief Decrypts the given text that was encrypted using a Caesar cipher with the specified shift.
     * 
     * @param text The text to be decrypted.
     * @param shift The shift value used in the Caesar cipher.
     * @return std::string The decrypted text.
     */
    string Decrypt(string_view text, const int &shift)
    {
        auto str = string(size(text), '\0');
        transform(cbegin(text), cend(text), begin(str), [&shift](const auto &ch) -> char {
            auto transformed_ch = ch;
            if (const auto loc = locale{}; isalpha(transformed_ch, loc))
            {
                const auto kA          = islower(transformed_ch, loc) ? 'a' : 'A';
                const auto kAlphaIndex = static_cast<int>(ch - kA);
                transformed_ch         = kA + ((26 + kAlphaIndex - shift) % 26);
            }
            return transformed_ch;
        });
        return str;
    }
}

int main()
{
    constexpr auto kText = string_view{ "abcdefghijklmnopqrstuvwxyz1234ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
    for (auto i = 0; i <= 26 ;++i)
    {
        auto encrypted = CaesarCipher::Encrypt(kText, i);
        auto decrypted = CaesarCipher::Decrypt(encrypted, i);
        if (decrypted != kText)
        {
            cout << "TEST CASE FAIL: \n";
            cout << "ORIGINAL TEXT : " << kText << '\n';
            cout << "ENCRYPTED TEXT: " << encrypted << '\n';
            cout << "DECRYPTED TEXT: " << decrypted << '\n';
            cout << "****************END OF TEST CASE******************\n";
        }
    }
    return 0;
}