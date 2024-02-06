/**
 * @file 16_enumerating_ipv4.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 *      Compilation command: g++ -std=c++17 16_enumerating_ipv4.cpp
 *      
 *      This file is solution to "Problem 16. Enumerating IPv4 addresses in a range"
 *      mentioned in "Chapter 2: Language Features" of the book:
 *      - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 *      Problem statement:
 *      "Write a program that allows the user to input two IPv4 addresses representing a range and
 *       list all the addresses in that range. Extend the structure defined for the previous problem to
 *       implement the requested functionality."
 *      
 *      This file implements a class `IPV4` which provides all the functionality for solving this
 *      problem i.e. constructing an IPV4 address, conversion to string, print on stream, taking
 *      input from stream, converting an IPV4 address to it integer value, comparing two IPV4
 *      addresses and incrementing and IPV4 address. See function for implementation details.
 * 
 * Driver code:
 *  Program first takes two IP addresses as input from user. Then prints all IP addresses starting from
 *  first IP address entered by the user upto second IP address entered by the user.
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <string>
#include <cstddef>
#include <sstream>
#include <locale>
#include <array>

using std::byte;
using std::cin;
using std::cout;
using std::endl;
using std::istream;
using std::ostream;
using std::string;
using std::stringstream;
using std::to_integer;
using std::array;

/**
 * @brief A class for representing an IPV4 address
 * 
 */
class IPV4
{
    using octet_type = unsigned char;
    octet_type octet1{0};
    octet_type octet2{0};
    octet_type octet3{0};
    octet_type octet4{0};

public:
    IPV4() = default;
    IPV4(const octet_type b1, const octet_type b2, const octet_type b3, const octet_type b4) : 
      octet1{b1},
      octet2{b2},
      octet3{b3},
      octet4{b4}
    {
    }

    string ToString() const
    {
        auto ss = stringstream{};
        ss << static_cast<int>(octet1) << '.'
           << static_cast<int>(octet2) << '.'
           << static_cast<int>(octet3) << '.'
           << static_cast<int>(octet4);
        return ss.str();
    }

    /**
     * @brief Converts an IPV4 address to its integer equivalent
     * Consider 172.16.254.1 as an example:
     * Since IPV4 is an 32-bit number grouped into 4 octets(bytes) 
     * First convert each octet into binary of 8 bits.
     * 172 = 10101100
     * 16  = 00010000
     * 254 = 11111110
     * 1   = 00000001
     * After writing binary together and converting to base-10 we get:
     * (10101100 00010000 11111110 00000001) base-2 = (2886794753) base-10
     * 
     *  @return uint32_t - integer representation of an IPV4 address
     */
    uint32_t ToInteger() const
    {
        auto value = (static_cast<uint32_t>(octet1) << 24) + (static_cast<uint32_t>(octet2) << 16)
            + (static_cast<uint32_t>(octet3) << 8) + static_cast<uint32_t>(octet4);
        return value;
    }

    bool operator<(const IPV4 &rhs) const
    {
        const auto &lhs = *this;
        return lhs.ToInteger() < rhs.ToInteger();
    }
    
    /**
     * @brief Increments an IPV4 address
     * The method increments the current IPV4 address by 1 and updates the individual octets
     * accordingly. It uses bitwise operations and predefined bit masks to manipulate the octets.
     * The operation is performed by adding 1 to the integer representation of the IPV4 address
     * and extracting the updated octet values using bit masks.
     * 
     * Example:
     * If the current IPV4 address is 192.168.1.255, calling Advance() will update it to 192.168.2.0.
     * 
     * @return IPV4& - increment the current object and returns its reference
     */
    IPV4& Advance()
    {
        constexpr auto a = 0b1111'1111'0000'0000'0000'0000'0000'0000;
        constexpr auto b = 0b0000'0000'1111'1111'0000'0000'0000'0000;
        constexpr auto c = 0b0000'0000'0000'0000'1111'1111'0000'0000;
        constexpr auto d = 0b0000'0000'0000'0000'0000'0000'1111'1111;

        const auto kValue = ToInteger() + 1;
        octet1 = (static_cast<uint32_t>(kValue) & a) >> 24;
        octet2 = (static_cast<uint32_t>(kValue) & b) >> 16;
        octet3 = (static_cast<uint32_t>(kValue) & c) >> 8;
        octet4 = (static_cast<uint32_t>(kValue) & d);
        
        return *this;
    }

    friend istream &operator>>(istream &in, IPV4 &address)
    {
        auto dot1 = '\0', dot2 = '\0', dot3 = '\0';
        auto octet1 = 0, octet2 = 0, octet3 = 0, octet4 = 0;
        in >> octet1 >> dot1 >> octet2 >> dot2 >> octet3 >> dot3 >> octet4;
        if ('.' == dot1 && '.' == dot2 && '.' == dot3) {
            address.octet1 = octet1;
            address.octet2 = octet2;
            address.octet3 = octet3;
            address.octet4 = octet4;
        }
        return in;
    }

    friend ostream &operator<<(ostream &out, const IPV4 &address)
    {
        out << address.ToString();
        return out;
    }
};

int main()
{
    auto address1 = IPV4{ };
    cout << "Enter address1: "; cin >> address1;

    auto address2 = IPV4{ };
    cout << "Enter address2: "; cin >> address2;

    while (address1 < address2)
    {
        cout << address1 << endl;
        address1.Advance();
    }
}