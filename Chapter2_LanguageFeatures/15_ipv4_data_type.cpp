/**
 * @file 15_ipv4_data_type.cpp
 * @author Usama Tayyab (usamtayyab9@gmail.com)
 * @brief 
 *      Compilation command : g++ -std=c++17 15_ipv4_data_type.cpp
 *      This file is solution to "Problem 15. IPv4 data type"
 *      mentioned in "Chapter 2: Language Features" of the book:
 *      - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *      
 *      The problem statement is:
 *      "Write a class that represents an IPv4 address. Implement the functions required to be able
 *      to read and write such addresses from or to the console. The user should be able to input
 *      values in dotted form, such as 127.0.0.1 or 168.192.0.100. This is also the form in
 *      which IPv4 addresses should be formatted to an output stream."
 *      
 *      This file implements a class `IPV4` which provides all the functionality for solving this
 *      problem i.e. constructing an IPV4 address, conversion to string, print on stream and taking
 *      input from stream.
 *      Driver code:
 *      An IPV4 object is constructed using an IP address and then printed on console. Afterwards
 *      an IPV4 adress is takes as input from user and the printed on the console.
 *      
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
};

template<class StreamType> 
StreamType& operator<<(StreamType &out, const IPV4 &address)
{
    out << address.ToString();
    return out;
}

int main()
{
    auto address1 = IPV4{192, 168, 1, 121};
    cout << address1.ToString() << endl;
    cout << "Please enter an IPv4 address:";
    cin >> address1;
    cout << address1 << endl;

    return 0;
}