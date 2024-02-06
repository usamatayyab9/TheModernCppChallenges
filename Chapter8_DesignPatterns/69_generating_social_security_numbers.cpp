/**
 * @file 69_generating_social_security_numbers.cpp
 * @author Usam Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 69_generating_social_security_numbers.cpp
 * This file is solution to "Problem 69. Generating social security numbers"
 *  mentioned in "Chapter 8 : Design Patterns" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * Three functions are provided for implementing this solution.
 * A generic function for generating a social security number(SSN)
 * `string GenerateSSN()`.
 * A function `GenerateNortheriaSSN` for generating northeria security numbers. Uses
 * `GenerateSSN` which northeria parameters.
 * A function `GenerateSoutherSSN` for generating southeria security numbers.
 * 
 * Driver code :
 * Generates and print SSN of both types.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <sstream>
#include <iomanip>
#include <random>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <numeric>

using std::accumulate;
using std::cbegin;
using std::cend;
using std::cout;
using std::endl;
using std::mt19937;
using std::random_device;
using std::ostringstream;
using std::setfill;
using std::setw;
using std::string;
using std::to_string;
using std::uniform_int_distribution;
using std::unordered_map;
using std::unordered_set;

enum class Gender { kMale, kFemale };

struct Date
{
    int day;
    int month;
    int year;
};
template <class Distribution, class Engine>
string GenerateSSN(const Gender &gender, const Date &birth_date,
                    const int &male_num, const int &female_num,
                    Distribution &dist , Engine &engine,
                    const int &modulus)
{
    auto osstream = ostringstream{};
    osstream << (Gender::kMale == gender ? male_num : female_num);
    osstream << birth_date.year << std::setw(2) << std::setfill('0') << birth_date.month << birth_date.day;
    static auto umap        = unordered_map<string, unordered_set<int>>{};
    const auto kBirthDayKey = to_string(birth_date.day).append(to_string(birth_date.month))
                            .append(to_string(birth_date.year));
    while (true)
    {
        auto number = dist(engine);
        if (auto &uset = umap[kBirthDayKey]; uset.find(number) == cend(uset))
        {
            uset.insert(number);
            osstream << number;
            break;
        }
    }
    auto number = osstream.str();
    auto index  = number.length() + 1;
    auto sum    = accumulate(cbegin(number), cend(number), 0, [&index](const auto &sum, const auto &ch)
    {
        const auto kDigit = ch - '\0';
        return sum + index-- * kDigit;
    });
    osstream << sum % modulus;
    return osstream.str();

}

string GenerateNortheriaSSN(const Gender &gender, const Date &birth_date)
{
    static auto rd     = random_device{};
    static auto engine = mt19937{ rd() };
    static auto dist   = uniform_int_distribution<>{ 10'000, 99'999 };
    return GenerateSSN(gender, birth_date, 7, 9, dist, engine, 10);

}

string GenerateSoutherSSN(const Gender &gender, const Date &birth_date)
{
    static auto rd     = random_device{};
    static auto engine = mt19937{ rd() };
    static auto dist   = uniform_int_distribution<>{ 1'000, 9'999 };
    return GenerateSSN(gender, birth_date, 1, 2, dist, engine, 11);
}


int main()
{
    cout << GenerateNortheriaSSN(Gender::kMale, Date{ 17, 7, 2023 }) << endl;
    cout << GenerateSoutherSSN(Gender::kMale, Date{ 17, 7, 2023 });
    return 0;
}