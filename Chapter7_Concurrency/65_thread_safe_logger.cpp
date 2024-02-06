/**
 * @file 65_thread_safe_logger.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Copilation command : g++ -std=c++17 65_thread_safe_logger.cpp -lpthread
 * 
 * This file is solution to "Problem 65. Thread-safe logging to the console"
 *  mentioned in "Chapter 7: Concurrency" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * A class named `Logger_mt` which takes a stream object by reference as constructor
 * argument. Provides a public `Log()` which utilizes the mutex for thread-safe logging.
 * 
 * Driver code:
 * 
 * For testing Logger_mt class. A logger attached to console stream
 * is instantiated. Afterwards 10 threads will be created 10ms delay.
 * Each thread will call a lambda which capture logger object by reference.
 * In lambda each thread will write its iteration number followed by a string "executing..."
 * For example:
 * 0 executing...
 * Once program finishes there should be no inter leaving characters.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <mutex>
#include <string_view>
#include <thread>
#include <chrono>
#include <thread>
#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::lock_guard;
using std::mutex;
using std::ostream;
using std::string_view;
using std::to_string;
using std::thread;
using std::this_thread::sleep_for;
using std::vector;
using namespace std::chrono_literals;

/**
 * @brief A multithreaded logger class.
 *
 * This class provides a thread-safe mechanism to log data to an output stream.
 *
 * @tparam OStreamType The type of the output stream.
 * @usage
 * auto logger  = Logger_mt{ cout };
 */
template<class OStreamType>
class Logger_mt
{
public:
    Logger_mt(OStreamType &output_stream_arg) : output_stream_{ output_stream_arg }
    {
    }
    template <class T>
    void Log(T data)
    {
        auto guard = lock_guard{ mutex_ };
        output_stream_ << data << endl;
    }

private:
    Logger_mt() = delete;
    /* mutex is non-copyable and non-assignable so logger class should also
        be non-copyable and non-assignable
    */
    Logger_mt(const Logger_mt &) = delete; 
    mutex        mutex_;
    OStreamType& output_stream_;
};


int main()
{
    /**
     * @brief
     * For testing Logger_mt class. A logger attached to console stream
     * is instantiated. Afterwards 10 threads will be created 10ms delay.
     * Each thread will call a lambda which capture logger object by reference.
     * In lambda each thread will write its iteration number followed by a string "executing..."
     * For example:
     * 0 executing...
     * Once program finishes there should be no inter leaving characters.
     */
    auto logger  = Logger_mt{ cout };
    auto threads = vector<thread>{};
    
    for (auto i = 0; i < 20 ;++i)
    {
        threads.push_back(
            thread{
                [&logger](int idx)
                {
                    sleep_for(10ms);
                    auto str = to_string(idx) + " executing...";
                    logger.Log(str);
                },
                i
            }
        );
    }

    for (auto &t : threads) { t.join(); }

    return 0;
}