/**
 * @file 22_temperature_scales.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 *  Compilation command : g++ -std=c++17 22_temperature_scales.cpp
 *  This file is solution to "Problem 22. Literals of various temperature scales"
 *  mentioned in "Chapter 2: Language Features" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 *  Problem statement:
 *  "Write a small library that enables expressing temperatures in the three most used scales,
 *   Celsius, Fahrenheit, and Kelvin, and converting between them. The library must enable you
 *   to write temperature literals in all these scales, such as 36.5_deg for Celsius, 97.7_f for
 *   Fahrenheit, and 309.65_K for Kelvin; perform operations with these values; and convert
 *   between them."
 * 
 * Solution:
 * First an enum `Scale` is declared which will be used to represent temperature in different scales i.e.
 * Celsius, Farhrenheit and Kelvin.
 * 
 * Then a class Temperature is defined which is takes a template argument of type `Scale`. Hence temperature
 * is able to represent temperatures like 1 celsius, 45 Farhrenheit, 370 kelvin etc.
 * 
 * Then to output `Temperature' class object stream output operator is overloaded with for Temperature class.
 * See function definition for implementation details.
 * 
 * Then a constexpr templated function `TemperatureCast()` is provided to convert temperatures from on scale to other.
 * 
 * Then a namespace temperature_scales is defined which defines all lieteral operators needed for creating
 * temperature values such as 100_deg, 37.5_f, 100_K etc.
 * 
 * Then a `Compare()` function is provided which takes two `Temperature` objects as argument and compares them
 * 
 * Driver code:
 * - Initializes some Temperature objects with different scales.
 * - Then cast the temperatures into a different scale and print theri values
 * - Lastly compares the different temperatures and prints theri comparison results. 
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <array>

using std::array;
using std::cout;
using std::endl;
using std::ostream;

/**
 * Extension :
 * Update Temperature class such that type of amount_ can also be templatized
 * such as int, long int, float or user defined arithmetic
 */

/**
 * @brief Enumeration of all scales for temperature
 * 
 */
enum class Scale { kCelsius, kFarhrenheit, kKelvin };

/**
 * @brief Represents a temperature value with a specified scale.
 * @tparam S - The temperature scale (e.g., Celsius, Fahrenheit, Kelvin).
 */
template<Scale S> class Temperature
{
    public:
    const Scale scale{ S };
    constexpr explicit Temperature(const double &arg) : amount_{ arg }
    {}
    constexpr explicit operator double() const { return amount_; }
    constexpr double Count() const { return amount_; }

    private:
    double amount_ { 0.0 };
};

/**
 * @brief Print Temperature objec @param t1 stream object @param out
 * 
 * @tparam StreamType 
 * @tparam scale 
 * @param out 
 * @param t1 
 * @return StreamType& - modified stream object after writing temperature on it
 */
template<class StreamType,Scale scale>
StreamType& operator<<(StreamType &out, const Temperature<scale> &t1)
{
    out << t1.Count();
    if constexpr      (Scale::kCelsius     == scale) { out << "C"; }
    else if constexpr (Scale::kFarhrenheit == scale) { out << "F"; }
    else if constexpr (Scale::kKelvin      == scale) { out << "K"; }

    return out;
}


/**
 * @brief A utility function for transforming a Temperature into a different scale.
 * 
 * Defines all the combination of functions in a table for converting between all three
 *  scales(celsius, Farhrenheit and kelvin).
 * Based on source scale type and destination scale type Fetches the conversion funtion
 * Passes the value into conversion function and returns the result.
 * 
 * @tparam Dest - unit to which @param temp is converted to
 * @tparam Src - unit of @param temp
 * @param temp - representing the which has to be converted into a different scale
 * @return Temperature<Dest> - converted temperature from one scale to another
 */
template <Scale Dest, Scale Src>
constexpr Temperature<Dest> TemperatureCast(const Temperature<Src> &temp)
{
    constexpr auto Identity = [](const double &arg) -> double { return arg; };    
    constexpr auto kConversionFunctions = array< array<double(*)(const double &), 3> ,3>
    {{
        {
            Identity,                                                        /* celsius to celsius */
            [](const double &value){ return ((value * 9.0) / 5.0) + 32.0; }, /* Celsius to Fahrenheit */
            [](const double &value){ return value + 273.15; }                /* celsius to Kelvin */
        },
        {
            [](const double &value){ return ((value - 32.0) * 5.0) / 9.0; },           /* Fahrenheit to celsius */
            Identity,                                                                  /* Fahrenheit to Fahrenheit */
            [](const double &value){ return ((value - 32.0) * 5.0 / 9.0) + 273.15; }  /* Fahrenheit to Kelvin */
        },
        {
            [](const double &value){ return value - 273.15; },                        /* kelvin to celsius */
            [](const double &value){ return ((value - 273.15) * 9.0 / 5.0) + 32.0; }, /* Kelvin to Fahrenheit */
            Identity                                                                  /* Kelvin to Kelvin */
        }
    }};
    constexpr auto kConversionFunction = kConversionFunctions[static_cast<int>(Src)][static_cast<int>(Dest)];
    return Temperature<Dest>( kConversionFunction(temp.Count()) );
}

/**
 * @brief Namespace for defining temperature literals
 * Literals are defined as follows:
 * _deg for Dgree celcisus
 * _f for Farhrenheit
 * _K for Kelvin
 */
namespace temperature_scales
{
    inline constexpr auto operator "" _deg(const long double arg)
    {
        return Temperature<Scale::kCelsius>{ static_cast<double>(arg) };
    }
    inline constexpr auto operator "" _f(const long double arg)
    {
        return Temperature<Scale::kFarhrenheit>{ static_cast<double>(arg) };
    }
    inline constexpr auto operator "" _K(const long double arg)
    {
        return Temperature<Scale::kKelvin>{ static_cast<double>(arg) };
    }
    inline constexpr auto operator "" _deg(const unsigned long long arg)
    {
        return Temperature<Scale::kCelsius>{ static_cast<double>(arg) };
    }
    inline constexpr auto operator "" _f(const unsigned long long arg)
    {
        return Temperature<Scale::kFarhrenheit>{ static_cast<double>(arg) };
    }
    inline constexpr auto operator "" _K(const unsigned long long arg)
    {
        return Temperature<Scale::kKelvin>{ static_cast<double>(arg) };
    }
}

/**
 * @brief compare two temperature values which may or may not be of same scale 
 *  Converts both the input arguments @param t1 and @param t2 into kelvin scale
 *  using @ref TemperatureCase.
 * @tparam lhs - Scale of param t1
 * @tparam rhs - Scale of param t2
 * @param t1
 * @param t2 
 * @return int
 *          -1 if t1 is less than t2
 *          0 if t2 is equal to t1
 *          1  if t1 is greater than t2 
 */
template<Scale lhs, Scale rhs>
constexpr int Compare(const Temperature<lhs> &t1, const Temperature<rhs> &t2)
{
    auto kT1Kelvin = TemperatureCast<Scale::kKelvin>(t1);
    auto kT2Kelvin = TemperatureCast<Scale::kKelvin>(t2);
    if (kT1Kelvin.Count() < kT2Kelvin.Count())  { return -1; }
    if (kT1Kelvin.Count() == kT2Kelvin.Count()) { return 0; }
    return 1;
}

using namespace temperature_scales;

int main()
{
    constexpr auto kC0 = 130.556_deg;
    constexpr auto kF0 = 267.001_f;
    constexpr auto kK0 = 1000_K;

    cout << kC0 << "  =  " << TemperatureCast<Scale::kCelsius>(kC0)     << endl;
    cout << kC0 << "  =  " << TemperatureCast<Scale::kFarhrenheit>(kC0) << endl;
    cout << kC0 << "  =  " << TemperatureCast<Scale::kKelvin>(kC0)      << endl;
    cout << endl;

    cout << kF0 << "  =  " << TemperatureCast<Scale::kCelsius>(kF0)     << endl;
    cout << kF0 << "  =  " << TemperatureCast<Scale::kFarhrenheit>(kF0) << endl;
    cout << kF0 << "  =  " << TemperatureCast<Scale::kKelvin>(kF0)      << endl;
    cout << endl;

    cout << kK0 << "  =  " << TemperatureCast<Scale::kCelsius>(kK0)     << endl;
    cout << kK0 << "  =  " << TemperatureCast<Scale::kFarhrenheit>(kK0) << endl;
    cout << kK0 << "  =  " << TemperatureCast<Scale::kKelvin>(kK0)      << endl;
    cout << endl;

    cout << Compare(kF0, kF0) << endl;
    cout << Compare(kC0, kC0) << endl;
    cout << Compare(kK0, kK0) << endl;

    return 0;
}