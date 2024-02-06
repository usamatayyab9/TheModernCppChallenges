/**
 * @file 21_system_handle_wrapper.cpp
 * @author Usam Tayyab (usamatayyab9@gmail.com)
 * @brief 
 *  Compilation command : g++ -std=c++17 21_system_handle_wrapper.cpp
 * This file is solution to "Problem 21. System handle wrapper"
 * mentioned in "Chapter 2: Language Features" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * "Consider an operating system handle, such as a file handle. Write a wrapper that handles
 * the acquisition and release of the handle, as well as other operations such as verifying the
 * validity of the handle and moving handle ownership from one object to another."
 * 
 * This file implements a class `ResourceHandler` which uses RAII techniques for managing 
 * file handles. The class is responsible for creating, moving, deleting, reading and writing to 
 * file descriptor. Copying a ResourceHandler object or assigning is disallowed.
 * 
 * Driver code:
 * ResourceHandler object is instantiated and then some data is written using class Write function.
 * Another ResourceHanlder object is instantiated and then some is read using class Read function
 * Printed to console. 
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <string_view>

#include <fcntl.h>
#include <unistd.h>

using std::cout;
using std::endl;
using std::string_view;

/**
 * @brief A simple resource manager class for file descriptors using RAII principles.
 */
class ResourceHandler
{
    public:
    ResourceHandler() = delete;
    ResourceHandler(const ResourceHandler &) = delete;
    ResourceHandler& operator=(const ResourceHandler &) = delete;

    /**
     * @brief Constructor to open a file with specified flags.
     * 
     * @param file - The name of the file.
     * @param flags - File opening flags.
     */
    ResourceHandler(string_view file, const int &flags)
    {
        fd = open(file.data(), flags);
        cout << "File descriptor: " << fd << endl;
    }

    /**
     * @brief Constructor to open a file with specified flags and mode.
     * 
     * @param file - The name of the file.
     * @param flags - File opening flags.
     * @param mode - File permissions when creating a new file.
     */
    ResourceHandler(string_view file, const int flags, const mode_t &mode)
    {
        fd = open(file.data(), flags, mode);
        cout << "File descriptor: " << fd << endl;
    }

    /**
     * @brief Move constructor.
     */
    ResourceHandler(ResourceHandler &&rref)
    {
        if (this != &rref)
        {
            this->Release();
            this->fd = rref.fd;
            rref.Release();
        }
    }

    /**
     * @brief Move assignment operator.
     */
    ResourceHandler& operator=(ResourceHandler &&rref)
    {
        if (this != &rref)
        {
            this->Release();
            this->fd = rref.fd;
            rref.Release();
        }
        return *this;
    }

    /**
     * @brief Writes data to the file.
     * 
     * @param data - The data to write.
     * @return ssize_t - The number of bytes written, or -1 if an error occurred.
     */
    ssize_t Write(string_view data)
    {
        if (kInvalidFd == fd)
        {
            return -1;
        }
        else
        {
            return write(fd, data.data(), data.size());
        }
    }

    /**
     * @brief Reads data from the file.
     * 
     * @param bytes_to_read - The number of bytes to read.
     * @return std::basic_string<unsigned char> - The read data.
     */
    std::basic_string<unsigned char> Read(const size_t &bytes_to_read)
    {
        if (kInvalidFd == fd)
        {
            return {};
        }
        else
        {
            auto bytes            = std::basic_string<unsigned char>(bytes_to_read, '\0');
            const auto kBytesRead = read(fd, bytes.data(), bytes_to_read);
            if (-1 == kBytesRead) { bytes.clear(); }
            return bytes;
        }
    }

    /**
     * @brief Destructor to release the file descriptor.
     */
    ~ResourceHandler()
    {
        Release();
    }
    
    /**
     * @brief Checks if the file descriptor is valid.
     * 
     * @return bool - True if valid, false otherwise.
     */
    bool IsValid() const
    {
        return kInvalidFd != fd;
    }
    private:
    /**
     * @brief Releases the file descriptor.
     */
    void Release()
    {
        if (IsValid())
        {
            close(fd);
            fd = kInvalidFd;
        }
    }
    static constexpr int kInvalidFd{ -1 };// Constant for an invalid file descriptor.
    int fd{ ResourceHandler::kInvalidFd };
};

int main()
{
    auto file_handle = ResourceHandler{ "example.txt", O_CREAT | O_WRONLY };
    file_handle.Write("ABCD");

    // auto file_handle_copy = file_handle; // error copy-constructor deleted
    auto file_handle3 = ResourceHandler{ "example.txt", O_RDONLY };
    auto data_read = file_handle3.Read(2);
    cout << data_read.size() << endl;
    cout << "Bytes read: " << data_read.data() << endl;

    // file_handle = file_handle3; // error assignment operator deleted
    return 0;
}