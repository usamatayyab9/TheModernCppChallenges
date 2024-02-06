/**
 * @file 39_function_execution_duration.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command: g++ -std=c++17 39_function_execution_duration.cpp
 * This file is solution to "Problem 39. Measuring function execution time"
 *  mentioned in "Chapter 5: Date and Time" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a function that can measure the execution time of a
 * function (with any number of arguments) in any required
 * duration (such as seconds, milliseconds, microseconds, and soon).
 * 
 * Solution:
 * This solution is implemented using a struct and function.
 * A MeasureFunctionDuration() which 3 template arguments a duration
 * type(seconds, milliseconds, microseconds etc), function 'F' whose 
 * execution time will be measured, variadic arguments neccessary for
 * invking 'F'.
 * This function returns a structure object of type
 * `FunctionDurationMeasureResult`. This struct contains two members
 * a duration object denoting the execution time of 'F' and return value
 * of 'F'. For function returning nothing i.e. void return type a
 * template specialization is provided for `FunctionDurationMeasureResult`
 * which only contains the execution duration.
 * See function and struct comments for more details.
 * 
 * After defining above two components multiple functions, struct
 * with member function and functors are defined each with a
 * different sleep interval. These are defined only for testing
 * the above function.
 * 
 * Driver code:
 * Calls MeasureFunctionDuration() multiple times with different 
 * functions and durations. After calling prints their execution
 * duration and in case of functions returning something also 
 * prints their return values.
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <chrono>
#include <functional>
#include <thread>
#include <cassert>
#include <type_traits>

using std::cout;
using std::chrono::duration_cast;
using std::chrono::steady_clock;
using std::invoke;
using std::invoke_result_t;
using std::this_thread::sleep_for;
using namespace std::chrono_literals;

template<class DurationType, class T>
struct FunctionDurationMeasureResult
{
    DurationType execution_duration;
    T            result;
};

template<class DurationType>
struct FunctionDurationMeasureResult<DurationType, void>
{
    DurationType execution_duration;
};

template<class DurationType, class F, class ...Args>
auto MeasureFunctionDuration(F &&f, Args&&...args)
{
    using function_return_type  = invoke_result_t<F, Args...>;
    auto duration_result        = FunctionDurationMeasureResult<DurationType, function_return_type>{};
    const auto kStartTimepoint  = steady_clock::now();
    if constexpr (true == std::is_void_v<function_return_type>) {
        invoke(f, args...);
    }
    else {
        duration_result.result = invoke(f, args...);
    }
    const auto kEndTimePoint    = steady_clock::now();
    duration_result.execution_duration = duration_cast<DurationType>(kEndTimePoint - kStartTimepoint);
    return duration_result;
}

void func1()
{
    cout << "Func1()...\n";
    sleep_for(1s);
}

int func3()
{
    cout << "Func3()...\n";
    sleep_for(1s);
    return 3;
}

void func4(int a)
{
    cout << "Func4(" << a << ")...\n";
    sleep_for(1s);
}

struct X {
    int func2()
    {
        cout << "X::func2()...\n";
        sleep_for(3s);
        return 0;
    }
};

struct X2
{
    void operator()(int i, float f)
    {
        cout << "X2::(" << i << ", " << f << ")...\n";
        sleep_for(2s);
    }
};

int main()
{
    
    auto result1 = MeasureFunctionDuration<std::chrono::seconds>(func1);
    cout << result1.execution_duration.count() << "s\n\n";

    auto result2 = MeasureFunctionDuration<std::chrono::milliseconds>(func3);
    cout << result2.execution_duration.count() << "ms\n";
    cout << "Function returned: " << result2.result << "\n\n";

    auto result3 = MeasureFunctionDuration<std::chrono::microseconds>(&X::func2, X{});
    cout << result3.execution_duration.count() << "us\n";
    cout << "Function returned: " << result3.result << "\n\n";

    auto result4 = MeasureFunctionDuration<std::chrono::nanoseconds>(func4, 56);
    cout << result4.execution_duration.count() << "ns\n\n";

    auto result5 = MeasureFunctionDuration<std::chrono::seconds>(X2{}, 100, 101.1);
    cout << result5.execution_duration.count() << "s\n\n";

    return 0;
}