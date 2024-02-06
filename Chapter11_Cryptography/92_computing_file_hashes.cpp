/**
 * @file 92_computing_file_hashes.cpp
 * @author Usama Tayyab (usamtayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 ./92_computing_file_hashes.cpp -lcryptopp
 * 
 * This file is solution to "Problem 92. Computing file hashes"
 * mentioned in "Chapter 11 : Cryptography" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * For solving this problem I used the crypto++ library which provides great documentation
 * and support for many cryptography functions. For this problem I wrote a function `GetHash()`
 * which uses crypto++ library for generating hash of a file. File path is provided as an argument.
 * Also which type hash to be generated is specified as template parameter. See function for
 * details on how crypto++ library is used. And also see `main()` on how to invoke the function
 * `GetHash()`.
 * 
 * Driver code:
 * - Program expects one additional argument. Providing more or less would result in printing a help
 * message. The required argument must be a valid file path otherwise program will print an error message 
 * about invalid file path.
 * - Once the file path is verified that it exists, calculates the SHA1, SHA256 and MD5 of the file
 * and prints them on console.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <string>
#include <string_view>
#include <filesystem>

#include "cryptopp/sha.h"
#include "cryptopp/files.h"
#include "cryptopp/hex.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptopp/md5.h"

using std::cout;
using std::endl;
using std::string;
using std::string_view;

/**
 * @brief Get the Hash of file specifed by the param path
 * 
 * @tparam SHA - Type of hashing to use
 * @param filename - file path for which hash will be generated
 * @return string - Hash string
 */
template<class SHA>
string GetHash(string_view filename)
{
    auto digest = string{};
    auto sha    = SHA{};
    CryptoPP::FileSource(filename.data(), true,
        new CryptoPP::HashFilter(sha,
            new CryptoPP::HexEncoder (
                new CryptoPP::StringSink(digest)
            )
        )
    );
    return digest;
}

int main(int argc, const char *args[])
{
    if (2 != argc) { cout << "Please provide a path to the file as an input to this program\n"; }
    else
    {
        auto filepath = string_view{ args[1] };
        if (auto ec = std::error_code{}; false == std::filesystem::exists(filepath, ec))
        {
            cout << "Please provide a valid file path\n";
        }
        else
        {
            cout << "SHA1   : " << GetHash<CryptoPP::SHA1>(filepath) << endl;
            cout << "SHA256 : " << GetHash<CryptoPP::SHA256>(filepath) << endl;
            cout << "MD5    : " << GetHash<CryptoPP::Weak::MD5>(filepath) << endl;
        }
    }
    return 0;
}