/**
 * @file 33_tabular_printing_processes.cpp
 * @author your name (you@domain.com)
 * @brief 
 *  Compilation command : g++ -std=c++17 33_tabular_printing_processes.cpp
 *  This file is solution to "Problem 33. Tabular printing of a list of processes"
 *  mentioned in "Chapter 4: Streams and Filesystems" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Suppose you have a snapshot of the list of all processes in a system. The information for
 * each process includes name, identifier, status (which can be either running or suspended),
 * account name (under which the process runs), memory size in bytes, and platform (which
 * can be either 32-bit or 64-bit). Your task is to write a function that takes such a list of
 * processes and prints them to the console alphabetically, in tabular format. All columns
 * must be left-aligned, except for the memory column which must be right-aligned. The value
 * of the memory size must be displayed in KB. The following is an example of the output of
 * this function:
 *              chrome.exe   1044    Running     marius.bancila  25180   32-bit
 *              chrome.exe   10100   Running     marius.bancila  227756  32-bit
 *              cmd.exe      512     Running     SYSTEM          48      64-bit
 *              explorer.exe 7108    Running     marius.bancila  29529   64-bit
 *              skype.exe    22456   Suspended   marius.bancila  656     64-bit
 * 
 * Solution:
 * To implement this solution different functions are provided. A struct `ProcessInfo` is defined for
 * which can represent the basic info about a process i.e. name, id, state(running or suspended) etc.
 * See struct comments for more details.
 * A function ProcessInfoPrettyPrinter() which takes a list of ProcessInfo struct and prints them in 
 * tabular form ordered alphabetically.
 * 
 * Driver code:
 * The program intializes a list of processes and passes it to function ProcessInfoPrettyPrinter().
 * 
 * @copyright Copyright (c) 2023
 */
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

using std::cout;
using std::endl;
using std::sort;
using std::string;
using std::vector;

/**
 * @brief A structure for denoting info about a process. 
 * 
 */
struct ProcessInfo
{
    enum class Status { kRunning, kSuspended };
    enum class Platform{ k32Bit, k64Bit };
    
    string      name;
    uint16_t    identifier;
    Status      state;
    string      owner;
    uintmax_t   memory_size;
    Platform    platform;
};

string ToString(const ProcessInfo::Status &state)
{
    auto str = string{ "N/A" };
    switch (state)
    {
        case ProcessInfo::Status::kRunning  : { str = "Running";   break; }
        case ProcessInfo::Status::kSuspended: { str = "Suspended"; break; }
        default                             : { break; } 
    }
    return str;
}

string ToString(const ProcessInfo::Platform &platform)
{
    auto str = string{ "N/A" };
    switch (platform)
    {
        case ProcessInfo::Platform::k32Bit : { str = "32-Bit"; break; }
        case ProcessInfo::Platform::k64Bit : { str = "64-Bit"; break; }
        default                            : { break; } 
    }
    return str;
}

/**
 * @brief Pretty-prints a vector of ProcessInfo structures in a tabular format.
 * @details
 * - Sorts the processes alphabetically
 * - Calculates the lenght of name of process with longest name
 * - Calculates the lenght of name of owenr with longest name
 * - Traverses the list @param processes and prints in tabular using left alignemnt, width setting,
 *      fill character setting.
 * @param processes - A vector of ProcessInfo structures to be printed.
 */
void ProcessInfoPrettyPrinter(vector<ProcessInfo> processes)
{
    sort(begin(processes), end(processes), [](const auto &p1, const auto &p2){
        return p1.name < p2.name;
    });
    const auto kMaxLenName  = std::max_element(cbegin(processes), cend(processes), [](const auto &p1, const auto &p2){ return size(p1.name) < size(p2.name); })->name.size();
    const auto kMaxLenOwner = std::max_element(cbegin(processes), cend(processes), [](const auto &p1, const auto &p2){ return size(p1.owner) < size(p2.owner); })->owner.size();

    for (const auto &process : processes)
    {
        cout << "| ";
        cout << std::setw(kMaxLenName)  << std::left  << std::setfill(' ') << process.name                         << " | ";
        cout << std::setw(5)            << std::left  << std::setfill(' ') << process.identifier                   << " | ";
        cout << std::setw(9)            << std::left  << std::setfill(' ') << ToString(process.state)              << " | ";
        cout << std::setw(kMaxLenOwner) << std::left  << std::setfill(' ') << process.owner                        << " | ";
        cout << std::setw(20)           << std::right << std::setfill(' ') << process.memory_size / 1024 << " KB " << " | ";
        cout << std::setw(5)            << std::left  << std::setfill(' ') << ToString(process.platform)           << " |";
        cout << '\n';
    }
}

int main()
{
    auto processes = vector<ProcessInfo>{
        { "a.out"       , 1104 , ProcessInfo::Status::kRunning  , "root"          , 123   , ProcessInfo::Platform::k32Bit },
        { "b.out"       , 1210 , ProcessInfo::Status::kSuspended, "usama"         , 12345 , ProcessInfo::Platform::k64Bit },
        { "c.out"       , 110  , ProcessInfo::Status::kRunning  , "usama"         , 15678 , ProcessInfo::Platform::k32Bit },
        { "d.out"       , 480  , ProcessInfo::Status::kSuspended, "root"          , 11000 , ProcessInfo::Platform::k32Bit },
        { "e.out"       , 687  , ProcessInfo::Status::kRunning  , "root"          , 180000, ProcessInfo::Platform::k32Bit },
        { "chrome.exe"  , 1044 , ProcessInfo::Status::kRunning  , "marius.bancila", 25180 , ProcessInfo::Platform::k32Bit },
        { "chrome.exe"  , 10100, ProcessInfo::Status::kRunning  , "marius.bancila", 227756, ProcessInfo::Platform::k32Bit },
        { "cmd.exe"     , 512  , ProcessInfo::Status::kRunning  , "SYSTEM"        , 48    , ProcessInfo::Platform::k64Bit },
        { "explorer.exe", 7108 , ProcessInfo::Status::kRunning  , "marius.bancila", 29529 , ProcessInfo::Platform::k64Bit },
        { "skype.exe"   , 22456, ProcessInfo::Status::kSuspended, "marius.bancila", 656   , ProcessInfo::Platform::k64Bit }
    };
    ProcessInfoPrettyPrinter(processes);
    return 0;
}