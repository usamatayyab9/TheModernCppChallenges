/**
 * @file 37_file_that_match_regex.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 37_file_that_match_regex.cpp
 * 
 * This file is solution to "Problem 37. Finding files in a directory that match a regular expression"
 *  mentioned in "Chapter 4: Streams and Filesystems" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a function that, given the path to a directory and a regular expression, 
 * returns a list of all the directory entries whose names match the regular expression.
 * 
 * Solution:
 * The function FindFilesThatMatchRegex() provides the solution to this problem.
 * See function comments for details.
 * 
 * Driver code:
 * 
 * The program expects to started with 2 additional arguments.
 * 1. First argument : should be path to a search directory
 * 2. Second argument: regular expression against which filenames will matched
 * 
 * Output: The program prints all entries of directiry that match regulat expression.
 * Each entry is printed on a new line.
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <filesystem>

using std::back_inserter;
using std::cin;
using std::copy_if;
using std::cout;
using std::endl;
using std::regex;
using std::regex_match;
using std::string;
using std::vector;
namespace std_fs = std::filesystem;

/**
 * @brief @brief Finds files in a directory and its subdirectories that match a given regular expression.
 * 
 * 
 * @param directory - directory path to search
 * @param re - The regular expression used to match file names
 * @return vector<std_fs::directory_entry> 
 */
vector<std_fs::directory_entry> FindFilesThatMatchRegex(const std_fs::path &directory, const regex &re)
{
    auto first      = std_fs::recursive_directory_iterator{ directory };
    auto last       = std_fs::recursive_directory_iterator{};
    auto files_list = vector<std_fs::directory_entry>{};
    copy_if(first, last, back_inserter(files_list), [&re](const std_fs::directory_entry &entry) {
        return entry.is_regular_file() && regex_match(entry.path().filename().string(), re);
    });
    return files_list;
}

int main(int argc, const char *args[])
{
    if (3 != argc)
    {
        cout << "This process expects two arguments.\n"
                "\tFirst argument : should be path to a directory\n"
                "\tSecond argument: regular expression\n"
                "output: All files in the directory(1st argument) that match the regular expression(2nd argument)\n";
    }
    else
    {
        cout << args[1] << endl;
        cout << args[2] << endl;
        const auto kFilesList = FindFilesThatMatchRegex(args[1], regex{ args[2] });
        for (const auto &entry : kFilesList)
        {
            cout << entry.path() << '\n';
        }       
    }
}