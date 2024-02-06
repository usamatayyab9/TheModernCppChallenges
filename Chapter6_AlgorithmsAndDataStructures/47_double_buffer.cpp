/**
 * @file 47_double_buffer.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 47_double_buffer.cpp -lpthread
 * This file is solution to "Problem 47. Double buffer"
 *  mentioned in "Chapter 6: Algorithms and Data Structures" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a class that represents a buffer that could be written and read at the same time
 * without the two operations colliding. A read operation must provide access to the old data
 * while a write operation is in progress. Newly written data must be available for reading
 * upon completion of the write operation.
 * 
 * Solution:
 * The class `DoubleBuffer` implements solution to this problem. The class contains
 * a mutable mutex and two vectors. One for reading and second for writing. Once
 * the write operation is being perfomed elements can be accessed from readers
 * vector. Once the write operation is completed readers vectors is swapped with
 * writers vector.
 * 
 * Driver code:
 * The program first initializes a double buffer of size 10
 * Then launches a thread which updates the buffer at regular interval.
 * In the main thread prints contents of buffer at regular interval for
 * 12 seconds.
 * Wait for the thread to fnish 
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <vector>
#include <mutex>
#include <algorithm>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

using std::begin;
using std::copy_n;
using std::cout;
using std::endl;
using std::lock_guard;
using std::mutex;
using std::min;
using std::size;
using std::chrono::steady_clock;
using std::swap;
using std::this_thread::sleep_for;
using std::thread;
using std::vector;

/**
 * @brief A double-buffer implementation for concurrent read and write operations.
 * 
 * This class provides a double-buffer mechanism, allowing concurrent write and read operations
 * without the need for external synchronization. It uses two internal buffers, one for writing
 * and another for reading. When a write operation is performed, the writer buffer is filled,
 * and then it swaps places with the reader buffer. Reading operations are performed on the
 * reader buffer. This allows reading to occur concurrently with writing without introducing
 * data races.
 * 
 * @tparam T The type of elements stored in the double buffer.
 */
template<class T>
class DoubleBuffer
{
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T &;

    explicit DoubleBuffer(const size_t &sz)
        : reader_buffer_(sz), writer_buffer_(sz)
    {
    }

    size_t Size() const { return reader_buffer_.size(); }
    void Write(const vector<T> src)
    {
        auto sz = std::min(size(writer_buffer_), size(src));
        auto guard = lock_guard(mutex_);
        copy_n(begin(src), sz, begin(writer_buffer_));
        writer_buffer_.swap(reader_buffer_);
    }

    reference operator[](const size_t idx)
    {
        auto guard = lock_guard(mutex_);
        return reader_buffer_[idx];
    }

    const_reference operator[](const size_t idx) const
    {
        auto guard = lock_guard(mutex_);
        return reader_buffer_[idx];
    }
    
    value_type ValueAt(const size_t idx)
    {
        auto guard = lock_guard(mutex_);
        auto val = reader_buffer_.at(idx);
        return val;
    }

private:
    vector<T> reader_buffer_;
    vector<T> writer_buffer_;
    mutable mutex mutex_;
};

template <class T>
void PrintBuffer(const DoubleBuffer<T> &buffer)
{
    const auto kSz = buffer.Size();
    for (auto idx = size_t{ 0 }; idx < kSz ;++idx)
    {
        cout << buffer[idx] << " ";
    }
}

int main()
{
    auto buffer = DoubleBuffer<int> (10);
    auto t      = thread([&buffer]()
    {
        for (auto i = 1; i < 1000; i += 10)
        {
            buffer.Write(vector{ i, i + 1, i + 2, i + 3, i + 4, i + 5, i + 6, i + 7, i + 8, i + 9 });
            sleep_for(100ms);
        }
    });

    const auto kStartTimepoint = steady_clock::now();
    do
    {
        PrintBuffer(buffer);
        cout << endl;
        sleep_for(150ms);
    } while (steady_clock::now() - kStartTimepoint < 12s);
    
    t.join();

    return 0;
}