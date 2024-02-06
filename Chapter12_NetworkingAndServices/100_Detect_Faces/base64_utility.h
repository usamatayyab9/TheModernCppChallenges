#ifndef BASE64_UTILITY_H
#define BASE64_UTILITY_H

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
    vector<bitset<8>> StringToBinaryData(string_view text)
    {
        return { text.begin(), text.end() };
    }
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

    string Encode(const vector<bitset<8>> &binary_data)
    {
        auto bits = string{};
        bits.reserve(8 * size(binary_data));
        for (const auto &kByte : binary_data)
        {
            bits += kByte.to_string();
        }
        const auto kRemainder           = size(bits) % 6;
        const auto kPaddingSymbolCount  = (0 == kRemainder) ? 0 : (6 - kRemainder);
        bits.insert(cend(bits), kPaddingSymbolCount, '0');

        // cout << "Padding symbol count: " << kPaddingSymbolCount << endl;
        // cout << "Bits: " << bits << ", size=" << size(bits) << endl;

        auto str = string{};
        for (auto idx = size_t{ 0 }; idx < size(bits); idx += 6)
        {
            auto start_ptr = bits.data() + idx;
            auto value     = 0;
            from_chars(start_ptr, start_ptr + 6, value, 2);
            str += kBase64EncodingTable[value];

            // cout << "str: " << std::string_view{ start_ptr, 6 } << ", value: " << value << ", encoding: " << kBase64EncodingTable[value] << endl;
        }
        str.insert(cend(str), kPaddingSymbolCount / 2, '=');
        return str;
    }
}

#endif // BASE64_UTILITY_H
