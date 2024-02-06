/**
 * @file 93_encrypting_decrypting_files.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 ./93_encrypting_decrypting_files.cpp -lcryptopp -static
 * 
 * This file is solution to "Problem 93. Encrypting and decrypting files"
 * mentioned in "Chapter 11 : Cryptography" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * For solving this problem I used crypto++ library. I used crypto++ `DefaultEncryptorWithMAC` and `DefaultDecryptorWithMAC`
 * for encryption and decryption respectively. See below function for more details.
 * 
 * Driver code :
 * - It checks whether the program is provided with the correct number of command-line arguments
 * (four arguments, including the program name). If not, it prints a message instructing the user
 * to provide three specific arguments: a valid file path, a password, and an output file path.
 * - If the correct number of arguments is provided, it checks if the specified input file exists using
 * the C++ filesystem library. If the file does not exist, it prints a message asking the user to provide
 * a valid file path.
 * - If the file exists, it extracts the file path, password, and output file path from the command-line
 * arguments. It then calls the Encrypt function with the provided file path, password, and output file path.
 * After encryption, it calls the Decrypt function with the output file path and the same password.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <string>
#include <string_view>
#include <filesystem>

#include "cryptopp/default.h"
#include "cryptopp/files.h"
#include "cryptopp/hex.h"

using std::cout;
using std::endl;
using std::string;
using std::string_view;

void Encrypt(string_view filepath, string_view password, string_view output_file)
{
    CryptoPP::FileSource(filepath.data(), true, 
        new CryptoPP::DefaultEncryptorWithMAC(password.data(),
            new CryptoPP::FileSink(output_file.data())
        )
    );
}

void Decrypt(string_view filepath, string_view password)
{
    CryptoPP::FileSource(filepath.data(), true, 
        new CryptoPP::DefaultDecryptorWithMAC(password.data(),
            new CryptoPP::FileSink("original.txt")
        )
    );
}

int main(int argc, const char *args[])
{
    if (4 != argc)
    {
        cout << "Please provide 3 input arugments:\n";
        cout << "1. A valid path to file which you want to encrypt\n";
        cout << "2. A password\n";
        cout << "3. An output filepath\n";

    }
    else
    {
        auto filepath = string_view{ args[1] };
        if (auto ec = std::error_code{}; false == std::filesystem::exists(filepath, ec))
        {
            cout << "Please provide a valid file path\n";
        }
        else
        {
            auto password = string_view{ args[2] };
            auto output   = string_view{ args[3] };
            Encrypt(filepath, password, output);
            Decrypt(output, password);

        }
    }
    return 0;
}