/**
 * @file 41_day_of_week.cpp
 * @author your name (you@domain.com)
 * @brief 
 * Compilation command :  g++ -std=c++17 41_day_of_week.cpp
 *  This file is solution to "Problem 41. Day of the week"
 *  mentioned in "Chapter 5: Date and Time" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a function that, given a date, determines the day
 * of the week. This function should return a value between
 * 1 (for Monday) and 7 (for Sunday).
 * 
 * Solution : 
 * This file utilizes the function WeekDayOf() to calulate
 * the week of day given date. See function for more details.
 * 
 * Driver code:
 * - The program first prints the week of my birthday
 * - Print the day on which I solved this problem
 * - Tests some assertion of different dates one of the is the
 *      birthday of ada lovlace.
 * - Prints week day of 21-Jan for different years. 21-Jan is
 *      birthday of my best friend.
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <array>
#include <utility>
#include <algorithm>
#include <string>
#include <cassert>

#include "date_utility_functions.h"

using std::array;
using std::cout;
using std::endl;
using std::pair;
using std::string;

int main()
{
    {
        constexpr auto kMyBirthDay               = Date{ 18, Month::kMarch, 1997 };
        constexpr auto kDateOfWritingThisProgram = Date{ 11, Month::kMarch, 2023 };
        cout << "I was born on " << ToString(WeekDayOf(kMyBirthDay)) << endl;
        cout << "I wrote this program on " << ToString(WeekDayOf(kDateOfWritingThisProgram)) << endl;
    }

    assert(WeekDayOf(Date{ 4,  Month::kJuly , 1911 }) == DaysOfWeek::kTuesday);
    assert(WeekDayOf(Date{ 18, Month::kMarch, 2030 }) == DaysOfWeek::kMonday);
    assert(WeekDayOf(Date{ 14, Month::kAug  , 1947 }) == DaysOfWeek::kThursday);
    assert(WeekDayOf(Date{ 10, Month::kDec  , 1815 }) == DaysOfWeek::kSunday); // Birthday of ada lovlace
    assert(WeekDayOf(Date{ 21, Month::kJan  , 1995 }) == DaysOfWeek::kSaturday);

    cout << ToString(WeekDayOf(Date{ 21, Month::kJan, 1995 })) << endl;
    cout << ToString(WeekDayOf(Date{ 21, Month::kJan, 2123 })) << endl;
    cout << ToString(WeekDayOf(Date{ 21, Month::kJan, 2223 })) << endl;
    cout << ToString(WeekDayOf(Date{ 21, Month::kJan, 2523 })) << endl;
    cout << ToString(WeekDayOf(Date{ 21, Month::kJan, 3023 })) << endl;

    return 0;
}