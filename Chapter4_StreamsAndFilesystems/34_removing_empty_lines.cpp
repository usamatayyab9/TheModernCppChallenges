/**
 * @file 34_removing_empty_lines.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 34_removing_empty_lines.cpp
 * This file is solution to "Problem 34. Removing empty lines from a text file"
 *  mentioned in "Chapter 4: Streams and Filesystems" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a program that, given the path to a text file, modifies the file by removing all empty
 * lines. Lines containing only whitespaces are considered empty.
 * 
 * Solution:
 * The function RemoveEmptyLinesFromFile() removes all empty lines from file provided as an argument.
 * 
 * Driver code:
 * The program expects only two arguments provided as command line argument. First one is by default
 * the name of the process, second is the name of the of which user intends to remove empty lines from.
 * Providing more or less than two arguments will result in program printing a help message.
 * 
 * @copyright Copyright (c) 2023
 */
#include <iostream>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <string>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::string_view;
namespace std_fs = std::filesystem;

/**
 * @brief Removes all empty lines from input file param filepath
 * - Opens the input file
 * - Creates a temporary file.
 * - Reads the input file line by line and copies into temporary file such that 
 *      omits the empty lines.
 * - Closes all the file handles.
 * - Replaces the @param filepath with the temporary file.
 * - Removes the temporary file.
 * NOTE: Leaves an empty line at end of file
 * @param filepath - path of file from which empty lines will be removed
 */
void RemoveEmptyLinesFromFile(const std_fs::path &filepath)
{
    if (auto fin = ifstream{ filepath }; false == fin.is_open()) { cout << "Unable to open file"; }
    else
    {
        const auto kTemporaryFile = std_fs::temp_directory_path() / "foo.txt";
        cout << "Temporary file: " << kTemporaryFile << '\n';
        if (auto fout = ofstream{ kTemporaryFile }; fout.is_open())
        {
            cout << "Output file opened\n";
            for (auto line = string{}; getline(fin, line) && fin;)
            {
                if (false == line.empty() && (string::npos != line.find_first_not_of(' ')))
                {
                    cout << line.size() << endl;
                    fout << line << '\n';
                }
            }
            fin.close();
            fout.close();
            std_fs::copy_file(kTemporaryFile, filepath, std_fs::copy_options::overwrite_existing);
            std_fs::remove(kTemporaryFile);
        }
    }
}

int main(int argc, const char *args[])
{
    if (2 != argc)
    {
        cout << "Invalid number of argument. This process expects a file path given" 
        "as command line argument and modifies the input file by removing empty lines from it.\n";
    }
    else
    {
        if (auto arg_view = string_view{ args[1] }; arg_view.empty())
        {
            cout << "Empty filename passed.\n";
        }
        else
        {
            RemoveEmptyLinesFromFile(arg_view);
        }
    }
    return 0;
}