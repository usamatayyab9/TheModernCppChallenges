/**
 * @file 94_file_signing.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 ./94_file_signing.cpp -lcryptopp -static
 * 
 * This file is solution to "Problem 94. File signing"
 * mentioned in "Chapter 11 : Cryptography" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * For solving this problem I used crypto++ library for creating RSA keys, creating signature file and verifying
 * signatures. See function for more details. Following functions are defined for this problem:
 * - `SaveKey()` : For saving RSA key to a file
 * - `LoadKey()` : For loading a key from file
 * - `VerifySignature()` : For verifying signature of a file. File path, public key and signature are provided
 *      as arguments.
 * - `CreateSignature()` : For creating a signature for a file. File path, private key and signature file are 
 * provided as argument.
 * 
 * Driver code:
 * This program has two modes: Sign or verify. Program expects 3 additional arguments:
 * 1) Mode of operation.
 * 2) Input file path
 * 3) Signature file path
 * 
 * - Command line Arguments check : It checks whether the program is provided with the correct number of
 *   command-line arguments (four arguments, including the program name). If not, it prints a message
 *   instructing the user to provide 3 additional arguments: mode ("sign" or "verify"), a valid file path,
 *   and path to signature file.
 * 
 * - Mode Check: If the correct number of arguments is provided, it checks if the specified mode is either
 *   "sign" or "verify." If the mode is valid, it proceeds with the corresponding operation.
 * 
 * - RSA Key Pair Generation and Signature Operation: If the mode is "sign," it generates an RSA key pair,
 *   saves the keys to files ("private.key" and "public.key"), and creates a signature for the specified
 *   input file using the private key. The signature is then saved to the specified signature file.
 * 
 * - RSA Key Pair Loading and Signature Verification: If the mode is "verify," it loads the public key from
 *   the "public.key" file, then verifies the signature of the specified input file using the loaded public
 *   key and the signature file. It prints whether the signature is valid or not.
 * 
 * - Invalid Mode Handling: If an invalid mode is provided, it prints a message indicating that an invalid
 *   operation was specified.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <string>
#include <string_view>
#include <filesystem>
//CryptoPP includes
#include "cryptopp/rsa.h"
#include "cryptopp/osrng.h"
#include "cryptopp/files.h"
#include "cryptopp/pssr.h"
#include "cryptopp/hex.h"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::string_view;

/**
 * Take from https://www.cryptopp.com/wiki/Keys_and_Formats#High_Level_Objects 
 * http://www.cryptopp.com/docs/ref/class_byte_queue.html
 * Template modifications are my own
 */
template<class KeyType> void SaveKey(const string& filename, const KeyType& key)
{
	auto queue = CryptoPP::ByteQueue{};
	key.Save(queue);
    auto file = CryptoPP::FileSink(filename.c_str());
	queue.CopyTo(file);
	file.MessageEnd();
}

/**
 * Take from https://www.cryptopp.com/wiki/Keys_and_Formats#High_Level_Objects 
 * Template modifications are my own
 */
template <class KeyType> void LoadKey(const string& filename, KeyType& key)
{
	auto queue = CryptoPP::ByteQueue{};
    auto file  = CryptoPP::FileSource(filename.c_str(), true /*pumpAll*/);
	file.TransferTo(queue);
	queue.MessageEnd();
	key.Load(queue);	
}

bool VerifySignature(string_view file, const CryptoPP::RSA::PublicKey &public_key, string_view signature_file)
{
    using namespace CryptoPP;
    auto result           = byte{ 0 };
    auto verifier         = RSASS<PSSR, SHA256>::Verifier{ public_key };
    auto filter           = SignatureVerificationFilter(verifier, new ArraySink(&result, sizeof(result)));
    auto signature_source = FileSource(signature_file.data(), true);
    auto file_source      = FileSource (file.data(), true);
    signature_source.TransferTo(filter);
    file_source.TransferTo(filter);
    filter.MessageEnd();
    return result;
}

void CreateSignature(string_view input_file, const CryptoPP::RSA::PrivateKey &private_key, string_view signature_file)
{
    using namespace CryptoPP;
    CryptoPP::AutoSeededRandomPool prng;
    auto signer = RSASS<PSSR, SHA256>::Signer(private_key);
    FileSource(input_file.data(), true,
        new SignerFilter(prng, signer,
            // new HexEncoder( /* To Hex encode signature, then Hex decode in verification too otherwise verification will fail*/
                new FileSink(signature_file.data())
            // )
        )
    );
}

struct RSA_KeyPair
{
    CryptoPP::RSA::PrivateKey private_key;
    CryptoPP::RSA::PublicKey  public_key;
};

RSA_KeyPair GenerateRSAKeyPair()
{
    using namespace CryptoPP;
    auto prng   = AutoSeededRandomPool{};
    auto params = InvertibleRSAFunction{};
    params.GenerateRandomWithKeySize(prng, 2048);
    // Create the keys
    auto privateKey = RSA::PrivateKey(params);
    auto publicKey  = RSA::PublicKey(privateKey);
    return { privateKey, publicKey };
}

int main(int argc, const char *args[])
{
    if (4 != argc)
    {
        cout << "Please provide correct number of arguments\n";
        cout << "First argument should be mode i.e. \"sign\" or \"verify\"\n";
        cout << "Second argument should be valid file path\n";
        cout << "Third argument should be path to a signature file. In case of sign mode signature will be stored at this path,\n";
        cout << "In case of verify mode signature will be loaded from this file path\n";
    }
    else if (auto mode = string_view{ args[1] }; "sign" == mode || "verify" == mode)
    {
        auto rsa_keys       = RSA_KeyPair{};
        auto input_file     = string_view{ args[2] };
        auto signature_file = string_view{ args[3] };
        if ("sign" == mode)
        {
            rsa_keys = GenerateRSAKeyPair();
            SaveKey("./private.key", rsa_keys.private_key); //Can be modified to save keys at user specified directory
            SaveKey("./public.key", rsa_keys.public_key);
            CreateSignature(input_file, rsa_keys.private_key, signature_file);
        }
        else if ("verify" == mode)
        {
            LoadKey("./public.key", rsa_keys.public_key);//Can be modified to load key from user specified directory
            cout << (VerifySignature(input_file, rsa_keys.public_key, signature_file) ? "Valid signature" : "Invalid signature") << "\n";
        }        
    }
    else
    {
        cout << "Invalid operation speicified";
    }
    return 0;
}