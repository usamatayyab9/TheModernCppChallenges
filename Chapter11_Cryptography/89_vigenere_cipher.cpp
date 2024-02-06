/**
 * @file 89_vigenere_cipher.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 89_vigenere_cipher.cpp
 * 
 *  This file is solution to "Problem 89. Vigenère cipher"
 *  mentioned in "Chapter 11 : Cryptography" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * For details on vigenere cipher read the below text taken from wikipedia(https://en.wikipedia.org/wiki/Vigen%C3%A8re_cipher):
 *      "To encrypt, a table of alphabets can be used, termed a tabula recta, Vigenère square or Vigenère table.
 *      It has the alphabet written out 26 times in different rows, each alphabet shifted cyclically to the left
 *      compared to the previous alphabet, corresponding to the 26 possible Caesar ciphers. At different points
 *      in the encryption process, the cipher uses a different alphabet from one of the rows. The alphabet used
 *      at each point depends on a repeating keyword. For example, suppose that the plaintext to be encrypted is:
 *          `attackatdawn`
 *      The person sending the message chooses a keyword and repeats it until it matches the length of the
 *      plaintext, for example, the keyword "LEMON":
 *          `LEMONLEMONLE`
 *      Each row starts with a key letter. The rest of the row holds the letters A to Z (in shifted order).
 *      Although there are 26 key rows shown, a code will use only as many keys (different alphabets) as there are
 *      unique letters in the key string, here just 5 keys: {L, E, M, O, N}. For successive letters of the message,
 *      successive letters of the key string will be taken and each message letter enciphered by using its
 *      corresponding key row. When a new character of the message is selected, the next letter of the key is
 *      chosen, and the row corresponding to that char is gone along to find the column heading that matches the
 *      message character. The letter at the intersection of [key-row, msg-col] is the enciphered letter. For
 *      example, the first letter of the plaintext, a, is paired with L, the first letter of the key. Therefore,
 *      row A and column L of the Vigenère square are used, namely L. Similarly, for the second letter of the
 *      plaintext, the second letter of the key is used. The letter at row T and column E is X. The rest of the
 *      plaintext is enciphered in a similar fashion:
 *          Plaintext:	attackatdawn
 *          Key:	LEMONLEMONLE
 *          Ciphertext:	LXFOPVEFRNHR
 *      Decryption is performed by going to the row in the table corresponding to the key, finding the position of
 *      the ciphertext letter in that row and then using the column's label as the plaintext. For example, in
 *      row L (from LEMON), the ciphertext L appears in column A, so a is the first plaintext letter. Next, in
 *      row E (from LEMON), the ciphertext X is located in column T. Thus t is the second plaintext letter."
 * 
 * For generating vigenere table following functions are used : `ConstexprIota()`, `ConstexprRotateCopy()`,
 * and `GetVigenerTable()`. `GetVigenerTable()` is defined in namespace `VigenereCipher` where other 
 * functions are defined outside of namespace. These functions are defined as constexpr because vigenere table
 * is predefined and can not be change during encryption and/or decryption, therefore it is marked as constexpr
 * so that it can be generated at compile time.
 * Functions for encryption and decryption are defined in namespace `VigenereCipher`.
 * 
 * Driver code :
 * - Calls the `TestCase()` function with different arguments and with different keys.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <array>
#include <numeric>
#include <string>
#include <algorithm>
#include <locale>
#include <cassert>

using std::array;
using std::begin;
using std::cbegin;
using std::cend;
using std::cout;
using std::end;
using std::endl;
using std::equal;
using std::find;
using std::iota;
using std::isalpha;
using std::islower;
using std::isupper;
using std::locale;
using std::toupper;
using std::size;
using std::string;
using std::string_view;

/** 
 * At time of solving this problem compilers didnt fully support C++20
 * Therefore I had to write my own version of constexpr iota.
*/
template <class IteratorType, class ValueType>
constexpr void ConstexprIota(IteratorType first, IteratorType last, ValueType value)
{
    while (first != last)
    {
        *first = value;
        ++first;
        ++value;
    }
}


/** 
 * At time of solving this problem compilers didnt fully support C++20
 * Therefore I had to write my own version of constexpr rotate_copy.
*/
template <class InputIteratorType, class OutputIteratorType>
constexpr OutputIteratorType ConstexprRotateCopy(InputIteratorType first, InputIteratorType middle, InputIteratorType last,
                                    OutputIteratorType dest)
{
    for (auto mid_iter = middle; mid_iter != last ;++mid_iter, ++dest)
    {
        *dest = *mid_iter;
    }
    while (first != middle)
    {
        *dest = *first;
        ++first;
        ++dest;
    }
    return dest;
}

namespace VigenereCipher
{
    /**
     * @brief Returns the Vigenere table used for encryption and decryption.
     * This table is generated by this function:
     * https://en.wikipedia.org/wiki/Vigen%C3%A8re_cipher#/media/File:Vigen%C3%A8re_square_shading.svg
     * @return constexpr std::array<std::array<char, 26>, 26> The Vigenere table.
     */
    constexpr auto GetVigenerTable()
    {
        auto table = array<array<char, 26>, 26>{};
        ConstexprIota(table[0].begin(), table[0].end(), 'A');
        for (auto idx = 1; idx < size(table) ;++idx)
        {
            auto &prev_arr = table[idx - 1];
            ConstexprRotateCopy(cbegin(prev_arr), cbegin(prev_arr) + 1, cend(prev_arr), begin(table[idx]));
        }
        return table;
    }

    /**
     * @brief Gets the index of the character in the alphabet.
     * 
     * @param ch The character.
     * @return int The index of the character in the alphabet (0-25). Returns -1 if the character is not an alphabet.
     */
    int GetAlphabetIndex(const char &ch)
    {
        auto idx = -1;
        if (islower(ch, locale{}))      { idx = static_cast<int>(ch - 'a'); }
        else if (isupper(ch, locale{})) { idx = static_cast<int>(ch - 'A'); }
        return idx;
    }

    /**
     * @brief Encrypts the plaintext using the Vigenere cipher.
     * 
     * @param plaintext The plaintext to be encrypted.
     * @param key The key for encryption.
     * @return std::string The encrypted ciphertext.
     */
    string Encrypt(string_view plaintext, string_view key)
    {
        static constexpr auto kVigenereTable = GetVigenerTable();
        auto ciphertext                      = string(size(plaintext), '\0');
        for (auto idx = size_t{ 0 }; idx < size(plaintext) ;++idx)
        {
            const auto &kKeyChar    = key[idx % size(key)];/*Moudlus for wrapping key character*/
            const auto &kTextChar   = plaintext[idx];
            const auto kKeyCharIdx  = GetAlphabetIndex(kKeyChar);
            const auto kTextCharIdx = GetAlphabetIndex(kTextChar);

            if ((-1 == kKeyCharIdx) || (-1 == kTextCharIdx)) { ciphertext[idx] = plaintext[idx];                            }
            else                                             { ciphertext[idx] = kVigenereTable[kKeyCharIdx][kTextCharIdx]; }
        }
        return ciphertext;
    }

    /**
     * @brief Decrypts the ciphertext using the Vigenere cipher.
     * 
     * @param ciphertext The ciphertext to be decrypted.
     * @param key The key for decryption.
     * @return std::string The decrypted plaintext.
     */
    string Decrypt(string_view ciphertext, string_view key)
    {
        static constexpr auto kVigenereTable = GetVigenerTable();
        auto plaintext                       = string(size(ciphertext), '\0');
        for (auto idx = size_t{ 0 }; idx < size(ciphertext) ;++idx)
        {
            const auto &kKeyChar    = key[idx % size(key)];
            const auto kKeyCharIdx  = GetAlphabetIndex(kKeyChar);
            const auto &kVigenereRow = kVigenereTable[kKeyCharIdx];
            auto pos                = std::find(cbegin(kVigenereRow), cend(kVigenereRow), ciphertext[idx]);
            
            if (pos != cend(kVigenereRow)) { plaintext[idx] = 'A' + (pos - cbegin(kVigenereRow)); }
            else                           { plaintext[idx] = ciphertext[idx];                    }
        }
        return plaintext;
    }
}

/**
 * @brief Test case function for VigenereCipher::Encrypt and VigenereCipher::Decrypt
 * 
 * @param plaintext The input plaintext string_view
 * @param key The key string_view used for encryption and decryption
 */
void TestCase(string_view plaintext, string_view key)
{
    auto encryptedtext = VigenereCipher::Encrypt(plaintext, key);
    auto decryptedtext = VigenereCipher::Decrypt(encryptedtext, key);
    cout << "TEST CASE\n";
    cout << "PLAIN TEXT    : " << plaintext << "\n";
    cout << "KEY           : " << key << "\n";
    cout << "ENCRYPTED TEXT: " << encryptedtext << "\n";
    cout << "DECRYPTED TEXT: " << decryptedtext << "\n\n";
    assert(
        equal(cbegin(plaintext), cend(plaintext), cbegin(decryptedtext), [](const auto &kPlainChar, const auto &kDecryptChar)
        {
            if (isalpha(kPlainChar, locale{}))
            {
                return toupper(kPlainChar) == kDecryptChar;
            }
            return kPlainChar == kDecryptChar;
        })
    );
}

int main()
{
    TestCase("attack at dawn", "LEMON");
    TestCase("The Modern CPP Challenge : Become an Expert programmer by solving 100 real world problems", "sample");
    TestCase("Mr. Javeria, Be happy, Be Healthy", "FRIEND");
    return 0;
}