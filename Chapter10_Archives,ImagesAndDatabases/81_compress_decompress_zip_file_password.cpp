/**
 * @file    81_compress_decompress_zip_file_password.cpp
 * @author  Usama Tayyab (usamatayyab9@gmail.com)
 * @brief
 * Compliation command for this program is :
 * 
 * g++ -std=c++17 81_compress_decompress_zip_file_password.cpp -Wl,-rpath=./ziplib/Bin  -L./ziplib/Bin -lzip
 * Where path to ziplib library(libzip.so) must be present in the current directory or the libarary path
 * should be updated in the above command. For this program an external library is used which can be
 * accessed at https://bitbucket.org/wbenny/ziplib/src. I only used this library for solving this problem.
 * 
 * This file is solution to "Problem 81. Compressing and decompressing files to/from a ZIP archive with a password"
 *  mentioned in "Chapter 10 : Archives, Images, and Databases" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * In this solution I used the ziplib library. This problem is similar to the previous except that this
 * requires password protection. For this purpose `ZipFile::AddEncryptedFile()` is used take password 
 * as argument.
 * 
 * Driver code:
 * This program expects only 4 additional arguments:
 *     1. Command which can either be "compress" or "decompress"
 *     2. Path which user wants to be compressed or decompressed
 *     3. Output where user want to store the result
 *     4. Password for compressing/decompressing data
 * All 4 arguments must be provided in a specific order i.e. command first, path second, output path third
 * password fourth.
 * All arguments should preceeded by a specifc prefixes. Providing more than or less than 4 arguments,
 * providing 4 arguments but not in fixed order or providing arguments but omitting or mistyping prefixes
 * will result in program doing nothing and printing the help prompt. 
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>
#include <string_view>
#include <filesystem>
#include <vector>
#include <regex>
#include <string>
#include <algorithm>
#include <iterator>

#include <functional>

#include "./ziplib/Source/ZipLib/ZipFile.h"

using std::cout;
using std::endl;
using std::filesystem::path;
using std::size;
using std::string;
using std::string_view;

inline constexpr auto kCommandArgumentPrefix        = string_view{ "command=" };
inline constexpr auto kSourcePathArgumentPrefix     = string_view{ "path=" };
inline constexpr auto kOutputPathArgumentPrefix     = string_view{ "output=" };
inline constexpr auto kPasswordArgumentPrefix       = string_view{ "password=" };

void PrintHelpPrompt()
{
    cout << "Invalid number of arguments provided\n";
    cout << "This program expects ONLY 4 arguments: 1) command to either compress to decompress 2) Path to file or directory 3) Output where result of compression or decompression will be saved 4) Password for compressing/decompressing data\n";
    cout << "All arguments must be in a specific order and with specific prefix\n";
    cout << "For example: command=compress/decompress path=<path to file/directory> output=<output path> password=123456 \n";
}

void Compress(const path &source, const path &target_path, string_view password)
{
    auto ec = std::error_code{};
    if (std::filesystem::exists(target_path, ec))
    {
        std::filesystem::remove(target_path, ec);
    }

    if (std::filesystem::is_directory(source))
    {
        auto CreateDirectoryInZipArchive = [](const path &target_zip_file, const path &in_archive_path){
            auto zip_archive = ZipFile::Open(target_zip_file);
            auto entry       = zip_archive->CreateEntry(in_archive_path);
            entry->SetAttributes(ZipArchiveEntry::Attributes::Directory);
            ZipFile::SaveAndClose(zip_archive, target_zip_file);
        };
        const auto kDirectoryName = source.stem();
        cout << "Directory Name: " << kDirectoryName << endl;
        CreateDirectoryInZipArchive(target_path, kDirectoryName);
        for (const auto &directory_entry : std::filesystem::recursive_directory_iterator{ source })
        {
            /*!
             * Reason for using lexically_relative path is that, for examples
               lets say user want to compress a directory like 
                /home
                    /user
                        /Folder
                            file1.cpp
                            /SubFolder1
                                sub_folder1.txt
                Then direcotry structure inside zip file would be like above i.e. it would contain
                /home and /user directories. To avoid this lexically_relative is used. So that 
                directory structure is like
                    Folder
                        file1.cpp
                        SubFolder
                            sub_folder.txt
             */
            const auto kInArchivePath = kDirectoryName / directory_entry.path().lexically_relative(source);
            cout << "compressing directory: " << kInArchivePath << endl;
            if (directory_entry.is_directory())
            {
                CreateDirectoryInZipArchive(target_path, kInArchivePath);
            }
            else if (directory_entry.is_regular_file())
            {
                ZipFile::AddEncryptedFile(target_path.string(), directory_entry.path().string(), kInArchivePath, password.data());
            }
        }
    }
    else
    {
        ZipFile::AddEncryptedFile(target_path.string(), source.string(), password.data());
    }
}

void Decompress(const path &source_zip_file, const path &target, string_view password)
{
    auto TestAndCreateDirectory = [](const path &p, std::error_code &ec) {
        if (false == std::filesystem::exists(p, ec))
        {
            std::filesystem::create_directories(p, ec);
        }
    };
    auto ec = std::error_code{};
    if (std::filesystem::exists(source_zip_file, ec) && std::filesystem::is_regular_file(source_zip_file, ec))
    {
        TestAndCreateDirectory(target, ec);
        auto zip_archive = ZipFile::Open(source_zip_file);
        cout << "Total " << zip_archive->GetEntriesCount() << " files will be decompressed" << endl;
        for (auto idx = size_t{ 0 }; idx < zip_archive->GetEntriesCount() ;++idx)
        {
            auto entry = zip_archive->GetEntry(idx);
            const auto kDestinationPath = target / entry->GetFullName();
            if (entry->IsDirectory())
            {
                TestAndCreateDirectory(kDestinationPath, ec);
            }
            else
            {
                ZipFile::ExtractEncryptedFile(source_zip_file, entry->GetFullName(), kDestinationPath, password.data());
            }
        }
    }
}

void CommandProcessor(string_view command, const path &source_path, const path target_path, string_view password)
{
    if ("compress" == command)
    {
        Compress(source_path, target_path, password);
    }
    else if ("decompress" == command)
    {
        Decompress(source_path, target_path, password);
    }
}

int main(int argc, const char *args[])
{
    if (argc != 5)
    {
        PrintHelpPrompt();
    }
    else
    {
        auto command_arg  = string_view{ args[1] };
        auto path_arg     = string_view{ args[2] };
        auto output_arg   = string_view{ args[3] };
        auto password_arg = string_view{ args[4] };
        if (0 == command_arg.find(kCommandArgumentPrefix) && 0 == path_arg.find(kSourcePathArgumentPrefix)
            && 0 == output_arg.find(kOutputPathArgumentPrefix) && 0 == password_arg.find(kPasswordArgumentPrefix))
        {
            const auto kCommand    = command_arg.substr(size(kCommandArgumentPrefix));
            const auto kPath       = path_arg.substr(size(kSourcePathArgumentPrefix));
            const auto kOutputPath = output_arg.substr(size(kOutputPathArgumentPrefix));
            const auto kPassword   =  password_arg.substr(size(kPasswordArgumentPrefix));
            if ("compress" == kCommand || "decompress" == kCommand)
            {
                CommandProcessor(kCommand, kPath, kOutputPath, kPassword);
            }
            else
            {
                cout << "Invalid command!!!\n";
            }
        }
        else
        {
            PrintHelpPrompt();
        }
    }
    return 0;
}