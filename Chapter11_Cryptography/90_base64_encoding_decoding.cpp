/**
 * @file 90_base64_encoding_decoding.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 90_base64_encoding_decoding.cpp
 * 
 * This file is solution to "Problem 90. Base64 encoding and decoding"
 *  mentioned in "Chapter 11 : Cryptography" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * For info on base-64 endocing scheme visit : https://en.wikipedia.org/wiki/Base64
 * 
 * The Base64 namespace provides functions for encoding and decoding data in Base64 format. It includes functions
 * for converting strings to binary data and vice versa, as well as functions for encoding and decoding Base64 data.
 * The implementation uses bitsets to represent binary data, and the Base64 encoding and decoding tables are defined
 * as constexpr arrays and an unordered_map, respectively. The Encode function converts binary data to a Base64-encoded
 * string, and the Decode function performs the reverse operation. Additionally, there are utility functions to convert
 * between strings and binary data.
 * 
 * Driver code:
 * - Initalizes a plain-text string
 * - Convert plain-text into binary data
 * - Encode binary data in base 64
 * - Then decodes the encoded the string to get binary data
 * - Then convert the binary data to retrieve the original plain-text string
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <cstddef>
#include <vector>
#include <string>
#include <bitset>
#include <array>
#include <charconv>
#include <cassert>
#include <unordered_map>
#include <numeric>
#include <algorithm>

using std::array;
using std::begin;
using std::bitset;
using std::byte;
using std::cbegin;
using std::cend;
using std::cout;
using std::endl;
using std::from_chars;
using std::size;
using std::string;
using std::string_view;
using std::unordered_map;
using std::vector;

namespace Base64
{
    /**
     * @brief Converts a string of characters to binary data represented as a vector of bitsets.
     * 
     * @param text The input string_view to be converted.
     * @return vector<bitset<8>> The binary data represented as a vector of bitsets.
    */
    vector<bitset<8>> StringToBinaryData(string_view text)
    {
        return { text.begin(), text.end() };
    }

    /**
     * @brief Converts binary data represented as a vector of bitsets to a string of characters.
     * 
     * @param data The binary data represented as a vector of bitsets.
     * @return std::string The resulting string of characters.
     */
    string BinaryDataToString(const vector<bitset<8>> &data)
    {
        auto str = string(size(data), '\0');
        std::transform(cbegin(data), cend(data), str.begin(), [](const bitset<8> &kByte) -> char {
            return kByte.to_ulong();
        });
        return str;
    }

    constexpr auto kBase64EncodingTable = array
    {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
    };

    /**
     * @brief Encodes binary data as a Base64 string.
     * 
     * @param binary_data The binary data to be encoded.
     * @return std::string The resulting Base64-encoded string.
     */
    string Encode(const vector<bitset<8>> &binary_data)
    {
        auto bits = string{};
        bits.reserve(8 * size(binary_data));
        /* Step 1: Convert binary data to a string of bits*/
        for (const auto &kByte : binary_data)
        {
            bits += kByte.to_string();
        }
        /* Step 2: Handle padding by adding zeros to make the total number of bits a multiple of 6*/
        const auto kRemainder           = size(bits) % 6;
        const auto kPaddingSymbolCount  = (0 == kRemainder) ? 0 : (6 - kRemainder);
        bits.insert(cend(bits), kPaddingSymbolCount, '0');
        auto str = string{};
        /* Step 3: Convert 6-bit groups to Base64 characters*/
        for (auto idx = size_t{ 0 }; idx < size(bits); idx += 6)
        {
            auto start_ptr = bits.data() + idx;
            auto value     = 0;
            from_chars(start_ptr, start_ptr + 6, value, 2);
            str += kBase64EncodingTable[value];            
        }
        /* Step 4: Add padding '=' characters*/
        str.insert(cend(str), kPaddingSymbolCount / 2, '=');
        return str;
    }

    static const auto kBase64DecodingTable = unordered_map<char, bitset<6>>{
        { 'A', 0b000000 }, { 'B', 0b000001 }, { 'C', 0b000010 }, { 'D', 0b000011 }, { 'E', 0b000100 }, { 'F', 0b000101 }, { 'G', 0b000110 },
        { 'H', 0b000111 }, { 'I', 0b001000 }, { 'J', 0b001001 }, { 'K', 0b001010 }, { 'L', 0b001011 }, { 'M', 0b001100 }, { 'N', 0b001101 },
        { 'O', 0b001110 }, { 'P', 0b001111 }, { 'Q', 0b010000 }, { 'R', 0b010001 }, { 'S', 0b010010 }, { 'T', 0b010011 }, { 'U', 0b010100 },
        { 'V', 0b010101 }, { 'W', 0b010110 }, { 'X', 0b010111 }, { 'Y', 0b011000 }, { 'Z', 0b011001 },
        { 'a', 0b011010 }, { 'b', 0b011011 }, { 'c', 0b011100 }, { 'd', 0b011101 }, { 'e', 0b011110 }, { 'f', 0b011111 }, { 'g', 0b100000 },
        { 'h', 0b100001 }, { 'i', 0b100010 }, { 'j', 0b100011 }, { 'k', 0b100100 }, { 'l', 0b100101 }, { 'm', 0b100110 }, { 'n', 0b100111 },
        { 'o', 0b101000 }, { 'p', 0b101001 }, { 'q', 0b101010 }, { 'r', 0b101011 }, { 's', 0b101100 }, { 't', 0b101101 }, { 'u', 0b101110 },
        { 'v', 0b101111 }, { 'w', 0b110000 }, { 'x', 0b110001 }, { 'y', 0b110010 }, { 'z', 0b110011 },
        { '0', 0b110100 }, { '1', 0b110101 }, { '2', 0b110110 }, { '3', 0b110111 }, { '4', 0b111000 }, { '5', 0b111001 }, { '6', 0b111010 },
        { '7', 0b111011 }, { '8', 0b111100 }, { '9', 0b111101 }, { '+', 0b111110 }, { '/', 0b111111 }
    };

    /**
     * @brief Decodes a Base64 string into binary data.
     * 
     * @param base64_data The Base64-encoded string.
     * @return std::vector<std::bitset<8>> The resulting binary data as a vector of bitsets.
     */
    vector<bitset<8>> Decode(string_view base64_data)
    {
        auto padding_symbols_count = 0;
        auto bits                  = string{};
        for (const auto &encoded_char : base64_data)
        {
            if ('=' == encoded_char) { ++padding_symbols_count; }
            else
            {
                bits += kBase64DecodingTable.at(encoded_char).to_string();
            }
        }
        bits.erase(cend(bits) - (2 * padding_symbols_count), cend(bits));/*Remove padding symbols from bits*/
        auto decoded = vector<bitset<8>>{};
        for (auto idx = size_t{ 0 }; idx < size(bits); idx += 8)
        {
            decoded.push_back(bitset<8>{ bits.data() + idx, 8 });
        }
        return decoded;
    }
}

int main()
{
    constexpr auto kText = string_view{ "Mr. Javeria, you are a good friend. Be Happy, Be healthy" };
    const auto kBinData  = Base64::StringToBinaryData(kText);
    const auto kEncoded  = Base64::Encode(kBinData);
    const auto kDecoded  = Base64::Decode(kEncoded);
    const auto kOriginal = Base64::BinaryDataToString(kDecoded);
    
    cout << "Original Text:  " << kText     << endl;
    cout << "After encoding: " << kEncoded  << endl;
    cout << "After decoding: " << kOriginal << endl;
    
    return 0;
}