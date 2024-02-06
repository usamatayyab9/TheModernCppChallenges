/**
 * @file    79_finding_files_in_zip_archive.cpp
 * @author  Usama Tayyab (usamatayyab9@gmail.com)
 * @brief
 * Compliation command for this program is :
 *     g++ -std=c++17 79_finding_files_in_zip_archive.cpp -Wl,-rpath=./ziplib/Bin  -L./ziplib/Bin -lzip
 * Where path to ziplib library(libzip.so) must be present in the current directory or the libarary path
 * should be updated in the above command. For this program an external library is used which can be
 * accessed at https://bitbucket.org/wbenny/ziplib/src. I only used this library for solving this problem.
 * 
 * This file is solution to "Problem 79. Finding files in a ZIP archive"
 *  mentioned in "Chapter 10 : Archives, Images, and Databases" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution :
 * A `SearchZipFiles()` which takes input two arguments a path to zip file and a string as regex.
 * And returns the list of all the file that matches the provided regex.
 * 
 * Driver code:
 * This program expects only two additional arguments:
 *     1. Path to zip file
 *     2. Regex as a search criteria
 * Both arguments must be provided in a specific order i.e. path first and regex second. Both arguments
 * should preceeded by a specifc prefixes. Providing more than 2 arguments, providing 2 arguments but not
 * in fixed order or providing arguments but omitting or mistyping prefixes will result in program doing
 * nothing and printing the help prompt. 
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

#include "./ziplib/Source/ZipLib/ZipFile.h"

using std::cbegin;
using std::cend;
using std::copy;
using std::cout;
using std::endl;
using std::filesystem::path;
using std::ostream_iterator;
using std::regex;
using std::regex_match;
using std::size;
using std::string;
using std::string_view;
using std::vector;

inline constexpr auto kCommandArgumentPrefix  = string_view{ "path=" };
inline constexpr auto kSourcePathArgumentPrefix = string_view{ "filter=" };

void PrintHelpPrompt()
{
    cout << "Invalid number of arguments provided\n";
    cout << "This program expects 2 arguments: 1) Path to a zip file 2) Regex for search criteria\n";
    cout << "Both arguments must be in a specific order and with specific prefix\n";
    cout << "For example: path=<zip_file_path> filter=<regex_for_filtering_files>\n";
}

auto SearchZipFiles(const path &zip_file_path, string_view regex_criteria)
{
    auto result = vector<string>{};
    if (auto ec = std::error_code{}; std::filesystem::exists(zip_file_path, ec))
    {
        auto zip_archive         = ZipFile::Open(zip_file_path.string());
        const auto kEntriesCount = zip_archive->GetEntriesCount();
        const auto kRegex        = regex{ regex_criteria.data() };
        for (auto idx = size_t{ 0 }; idx < kEntriesCount ;++idx)
        {
            auto entry = zip_archive->GetEntry(idx);
            if ((false == entry->IsDirectory()) && regex_match(entry->GetName(), kRegex))
            {
                result.push_back(entry->GetFullName());
            }
        }
    }
    return result;
}

int main(int argc, const char *args[])
{
    if (argc != 3)
    {
        PrintHelpPrompt();
    }
    else
    {
        if (auto path = string_view{ args[1] }, regex_arg = string_view{ args[2] };
            0 == path.find(kCommandArgumentPrefix) && 0 == regex_arg.find(kSourcePathArgumentPrefix))
        {
            const auto kZipFilePath   = path.substr(size(kCommandArgumentPrefix));
            const auto kRegex         = regex_arg.substr(size(kSourcePathArgumentPrefix));
            const auto kMatchingFiles = SearchZipFiles(kZipFilePath, kRegex);
            copy(cbegin(kMatchingFiles), cend(kMatchingFiles), ostream_iterator<string>{ cout, "\n" });
        }
        else
        {
            PrintHelpPrompt();
        }
    }
    return 0;
}