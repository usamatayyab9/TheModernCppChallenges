/**
 * @file 35_computing_size_of_directory.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 35_computing_size_of_directory.cpp
 * This file is solution to "Problem 35. Computing the size of a directory"
 *  mentioned in "Chapter 4: Streams and Filesystems" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a function that computes the size of a directory, in bytes, recursively. It should be
 * possible to indicate whether symbolic links should be followed or not.
 * 
 * Solution:
 * The function SizeOfDirectory() calculates the size of directory whose path is given as the first 
 * argument to the function and decision whether to follow the symlinks or not is provided as 
 * second parameter to the function.
 * 
 * Driver code:
 * Program expects 2 arguments:
 *      - First argument should be a directory path
 *      - Second argument should be a boolean value of 0 or 1.
 *          1 means symlinks will be followed, 0 means symlinks will not be followed.
 * Outputs size of directory in bytes.
 * 
 * @copyright Copyright (c) 2023
 */
#include <iostream>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <string>
#include <iomanip>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::quoted;
using std::string;
using std::string_view;
namespace std_fs = std::filesystem;

using std_fs::directory_options;

/**
 * @brief 
 * [TODO] If directory contains a symlink that could lead
 * back to same directory i.e. recursive symlink, then
 * function would end up in an infinit loop. Update it
 * such that recursive symlinks are catered.
 * 
 * @param directory 
 * @param is_symlinks_allowed 
 * @return size of param directory in bytes
 */
auto SizeOfDirectory(const std_fs::path &directory, const bool &is_symlinks_allowed)
{
    auto total_bytes = uintmax_t{ 0 };
    for (const auto &entry : std_fs::recursive_directory_iterator{ directory, is_symlinks_allowed ? 
                                                                             directory_options::follow_directory_symlink : 
                                                                             directory_options::none }
    )
    {
        if (entry.is_regular_file()) {
            total_bytes += entry.file_size();
        }
    }
    return total_bytes;
}

int main(int argc, const char *args[])
{
    if (3 != argc)
    {
        cout << "Invalid number of argument. This process expects 2 arguments:\n";
        cout << "\tFirst argument should be a directory path\n";
        cout << "\tSecond argument should be a boolean value of 0 or 1. 1 means symlinks will be followed,"
        "0 means symlinks will not be followed\n";
        cout << "Outputs size of directory in bytes.\n";
    }
    else
    {
        auto arg_view = string_view{ args[1] };
        if (arg_view.empty()) { cout << "Empty filename passed.\n"; }
        else
        {
            if (auto symlink_arg = string_view{ args[2] }; symlink_arg.empty() || (symlink_arg != "0" && symlink_arg != "1"))
            {
                cout << "Invalid second argument provided\n";
            }
            else
            {
                const auto kSz = SizeOfDirectory(arg_view, symlink_arg == "1");
                cout << quoted(arg_view) << " contains " << kSz << " bytes\n";
            } 
        }
    }
    return 0;
}