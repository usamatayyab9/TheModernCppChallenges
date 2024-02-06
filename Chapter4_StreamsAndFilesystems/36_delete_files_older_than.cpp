/**
 * @file 36_delete_files_older_than.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 36_delete_files_older_than.cpp
 * 
 * This file is solution to "Problem 36. Deleting files older than a given date"
 *  mentioned in "Chapter 4: Streams and Filesystems" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a function that, given the path to a directory and a duration, deletes all the entries
 * (files or subdirectories) older than the specified duration, in a recursive manner. The
 * duration can represent anything, such as days, hours, minutes, seconds, and so on, or a
 * combination of that, such as one hour and twenty minutes. If the specified directory is itself
 * older than the given duration, it should be deleted entirely.
 * 
 * Solution:
 * Two function are provided for implementing the solution:
 * - IsOlderThanDuration() which test whether the specified path(passed as first argument) is
 *      modified before the certain duration(passed as second argument).
 * - RemoveFilesOlderThan() Traverses the path(provided as first argument) and removes all
 *      files that are modified before certain duration(provided as second argument).
 * 
 * Driver code:
 * - Calls the above function.
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <string_view>
#include <thread>
#include <chrono>

using std::back_inserter;
using std::cin;
using std::cout;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::system_clock;
using std::endl;
using std::ofstream;
using std::string_view;
using namespace std::chrono_literals;
namespace std_fs = std::filesystem;

/**
 * @brief Checks if a file at the given path is older than the specified duration.
 * 
 * This function retrieves the last write time of the file specified by the provided path,
 * calculates the duration since the last write time, and compares it with the given duration.
 * If the file is older than the specified duration, the function returns true; otherwise, false.
 * 
 * @tparam DurationType The type of duration used for the comparison (e.g., std::chrono::hours).
 * @param path The path to the file.
 * @param dur The duration to compare against.
 * @return true if the file is older than the specified duration, false otherwise.
 */
template <class DurationType>
bool IsOlderThanDuration(const std_fs::path &path, const DurationType &dur)
{
    using FileClock = std_fs::file_time_type::clock;
    const auto kLastWriteTime          = std_fs::last_write_time(path);
    const auto kDurationSinceLastWrite = duration_cast<DurationType>(FileClock::now() - kLastWriteTime);
    return kDurationSinceLastWrite > dur;
}

/**
 * @brief Recursively removes files and directories older than the specified duration.
 * 
 * This function takes a directory path and a duration and recursively removes files and
 * directories within the specified directory that are older than the given duration.
 * It uses the `IsOlderThanDuration` function to check each file if it is modified before
 * @param duration before removal.
 * 
 * @tparam DurationType The type of duration used for the age comparison (e.g., std::chrono::hours).
 * @param path The path to the directory to be scanned and files removed.
 * @param duration The duration threshold; files older than this duration will be removed.
 */
template <class DurationType>
void RemoveFilesOlderThan(const std_fs::path &path, const DurationType &duration)
{
    if (std_fs::exists(path))
    {
        if (std_fs::is_directory(path))
        {
            for (auto const &entry : std_fs::directory_iterator(path))
            {
                RemoveFilesOlderThan(entry.path(), duration);
            }
            std_fs::remove_all(path);
        }
        else if (IsOlderThanDuration(path, duration))
        {
            cout << "Removing: " << path << endl;
            std_fs::remove(path);
        }
    }
}

int main(int argc, const char *args[])
{
    RemoveFilesOlderThan("./Test/", 1h);
    return 0;
}