/**
 * @file 38_temporary_log_file.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command: g++ -std=c++17 38_temporary_log_file.cpp
 * This file is solution to "Problem 38. Temporary log files"
 *  mentioned in "Chapter 4: Streams and Filesystems" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Create a logging class that writes text messages to a discardable text file.
 * The text file should have a unique name and must be located in a temporary
 * directory. Unless specified otherwise, this log file should be deleted when
 * the instance of the class is destroyed. However, it should be possible to
 * retain the log file by moving it to a permanent location.
 * 
 * Solution:
 * - class `Logger` is implemeted as solution to this problem. This class
 *  provides all the functionality for creating a temporary log file,
 *  writing data to log file, making temporary file a permanent one,
 *  removing the temporary log file, fetching the log file path.
 *  See class function comments for more details.
 * 
 * Driver code:
 * The program starts by instantiating an object of Logger class. Writes
 * some data to it, prints logger file path on console, make it permanent,
 * prints its path, again write log data. 
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

using std::back_inserter;
using std::cin;
using std::cout;
using std::endl;
using std::ofstream;
using std::string_view;
namespace std_fs = std::filesystem;

/**
 * @brief A basic class for creating logger
 * 
 */
class Logger
{
public:
    Logger() : file_path_{ std_fs::temp_directory_path() / kFilename_ }
    {
        fout_.open(file_path_);
    }

    /**
     * @brief Removes the temporary logger file and sets param destination_filepath
     * as permanent file for logging
     * 
     * @param destination_filepath - path of permanent logger file
     */
    void MakeItPermanent(const std_fs::path &destination_filepath)
    {
        fout_.close();
        std_fs::copy_file(file_path_, destination_filepath);
        std_fs::remove(file_path_);
        file_path_ = destination_filepath;
        fout_.open(file_path_, std::ios_base::app);
        remove_ = false;
    }

    template<class T>
    Logger& operator<<(T data)
    {
        fout_ << data;
        return *this;
    }

    std_fs::path Path() const
    {
        return file_path_;
    }

    ~Logger() noexcept
    {
        if (fout_ && fout_.is_open())
        {
            fout_.close();
        }
        if (remove_ && std_fs::exists(file_path_))
        {
            std_fs::remove(file_path_);
        }
    }

private:
    const string_view kFilename_{ "foo.txt" };
    std_fs::path      file_path_;
    ofstream          fout_;
    bool              remove_{ true };
};

int main()
{
    auto logger = Logger{};
    cout << "Logger at " << logger.Path() << '\n';
    cout << "Logging data...";
    logger << "Usama" << " " << "Tayyab\n" << 2 << '\n';
    logger.MakeItPermanent("./abc.txt");
    cout << "Logger at " << logger.Path() << '\n';
    logger << 3 << 4;

    return 0;
}