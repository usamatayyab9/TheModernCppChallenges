/**
 * @file 40_days_between_two_dates.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 40_days_between_two_dates.cpp
 * This file is solution to "Problem 40. Number of days between two dates"
 *  mentioned in "Chapter 5: Date and Time" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a function that, given two dates, returns the number of days
 * between the two dates. The function should work regardless of the
 * order of the input dates
 * 
 * Solution: 
 * 
 * This problem is solved using function DaysBetweenDates() which takes
 * two dates as input and returns the number of days between.
 * For more details see headers date.h and date_utility_functions.h.
 * 
 * DaysBetweenDates() is used in other problems of this chapter
 * therefore it is put in a header from where it can be used in
 * other files too.
 * 
 * Driver code:
 * 
 * Program first calculates the number of days between my birthday
 * and the day when I wrote this program.
 * Then prints the number of days since my birthday current day.
 * Then calculates some random days between different dates and
 * comapres them then in as assertion with their original truth
 * values.
 * 
 * @copyright Copyright (c) 2023
 */
#include <iostream>
#include <array>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <ctime>
#include <cassert>

#include "date.h"
#include "date_utility_functions.h"

using std::array;
using std::cout;
using std::endl;
using std::chrono::system_clock;

int main()
{
    constexpr auto kMyBirthday               = Date{ 18, Month::kMarch, 1997 };
    constexpr auto kDayWhenIWroteThisProgram = Date{ 6, Month::kMarch, 2023 };
    const auto kDaysFromBirthToWritingThisProgram = DaysBetweenDates( kMyBirthday, kDayWhenIWroteThisProgram);
    cout << "I was " << kDaysFromBirthToWritingThisProgram << " days old when I wrote this program" << endl;

    {
        const auto kTime  = system_clock::to_time_t(system_clock::now());
        auto t            = std::localtime(&kTime);
        const auto kToday = Date{ t->tm_mday, static_cast<Month>(t->tm_mon), t->tm_year + 1900 };
        cout << "Today I am " << DaysBetweenDates(kMyBirthday, kToday) << " days old" << endl;
    }

    assert(DaysBetweenDates(Date{ 17, Month::kNov , 2010 }, Date{ 7 , Month::kMarch, 2023 } )    == 4493);
    assert(DaysBetweenDates(Date{ 13, Month::kOct , 1930 }, Date{ 7 , Month::kMarch, 2023 } )    == 33748);
    assert(DaysBetweenDates(Date{ 1 , Month::kJan , 1970 }, Date{ 27, Month::kApril, 2020 } )    == 18379);
    assert(DaysBetweenDates(Date{ 5 , Month::kJune, 1856 }, Date{ 17, Month::kMay  , 2003 } )    == 53671);
    assert(DaysBetweenDates(Date{ 19, Month::kAug , 1914 }, Date{ 9 , Month::kMarch, 2023 } )    == 39649);
    assert(DaysBetweenDates(Date{ 19, Month::kAug , 1914 }, Date{ 19, Month::kAug  , 1915 } )    == 365);
    assert(DaysBetweenDates(Date{ 19, Month::kAug , 1914 }, Date{ 17, Month::kSept , 1938 } )    == 8795);
    assert(DaysBetweenDates(Date{ 19, Month::kAug , 1914 }, Date{ 20, Month::kAug  , 1914 } )    == 1);
    assert(DaysBetweenDates(Date{ 1 , Month::kJan , 1610 }, Date{ 31, Month::kDec  , 2549 } )    == 343327);
    assert(DaysBetweenDates(Date{ 1 , Month::kJan , 1601 }, Date{ 31, Month::kDec  , 2549 } )    == 346614);
    assert(DaysBetweenDates(Date{ 1 , Month::kJan , 2023 }, Date{ 31, Month::kDec  , 2023 } )    == 364);
    assert(DaysBetweenDates(Date{ 1 , Month::kJan , 2024 }, Date{ 31, Month::kDec  , 2024 } )    == 365);
    assert(DaysBetweenDates(Date{ 1 , Month::kJan , 1970 }, Date{ 15, Month::kOct  , 2007 } )    == 13801);
    assert(DaysBetweenDates(Date{ 4 , Month::kJuly, 1911 }, Date{ 1 , Month::kJan  , 1970 } )    == 21366);

    return 0;
}