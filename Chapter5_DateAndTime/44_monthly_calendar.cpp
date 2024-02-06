/**
 * @file 44_monthly_calendar.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 44_monthly_calendar.cpp
 * 
 * This file is solution to "Problem 44. Monthly calendar"
 *  mentioned in "Chapter 5: Date and Time" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a function that, given a year and month, prints to
 * the console the month calendar. The expected output
 * format is as follows (the example is for December 2017):
 *                  Mon Tue Wed Thu Fri Sat Sun
 *                                    1   2   3
 *                    4   5   6   7   8   9  10
 *                   11  12  13  14  15  16  17
 *                   18  19  20  21  22  23  24
 *                   25  26  27  28  29  30  31
 * 
 * Solution :
 * This solution is implemented using two functions
 * MoveToNextDay() shift the given day to the next.
 * For example:
 * Mon -> Tue
 * Tue -> Wed
 * ...
 * Sat -> Sun
 * Sun -> Mon
 * 
 * PrintMonthlyCalenday() prints the monthly calendar on console.
 * This function is responsible for controlling empty spaces,
 * adding linebreak after sunday etc.
 * 
 * Driver code:
 * 
 * The program prints the monthly calendar of December 2017
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <sstream>
#include <utility>
#include <iomanip>

#include "date.h"
#include "date_utility_functions.h"

using std::cout;
using std::endl;
using std::pair;
using std::stringstream;

/**
 * @brief Moves to the next day in the sequence of days (Monday to Sunday).
 * 
 * This function updates the given day to the next day in the sequence, considering the order
 * of days in a week (Monday, Tuesday, ..., Sunday). If the input is Sunday, the function sets
 * the day to Monday, completing the cycle.
 * 
 * @param[in,out] day - The day to be incremented to the next day in the sequence.
 */
void MoveToNextDay(DaysOfWeek &day)
{
    switch (day)
    {
        case DaysOfWeek::kMonday    : { day = DaysOfWeek::kTuesday;   break; }
        case DaysOfWeek::kTuesday   : { day = DaysOfWeek::kWednesday; break; }
        case DaysOfWeek::kWednesday : { day = DaysOfWeek::kThursday;  break; }
        case DaysOfWeek::kThursday  : { day = DaysOfWeek::kFriday;    break; }
        case DaysOfWeek::kFriday    : { day = DaysOfWeek::kSaturday;  break; }
        case DaysOfWeek::kSaturday  : { day = DaysOfWeek::kSunday;    break; }
        case DaysOfWeek::kSunday    : { day = DaysOfWeek::kMonday;    break; }
        default : { break; }
    }
}

/**
 * @brief Prints a monthly calendar for the specified month and year.
 * 
 * This function prints a formatted calendar for the given month and year, displaying each day
 * of the month with corresponding weekdays in a grid format. It takes into account the starting
 * day of the week for the first day of the month.
 * 
 * @param month The month for which the calendar is to be printed.
 * @param year The year for which the calendar is to be printed.
 */
void PrintMonthlyCalenday(const Month &month, const int &year)
{
    const auto kNumberOfDays = DaysInMonth(month, year);
    const auto kFirstDay     = WeekDayOf(Date{ 1, month, year });
    auto ss                  = stringstream{};
    
    ss << "Mon Tue Wed Thu Fri Sat Sun\n";
    /*! Print empty entries in 1st row. It is possible that 1st of @param month, @param year
        appears on lets say Friday.
    */
    for (const auto kDayOfWeek : kDaysList)
    {
        if (kDayOfWeek == kFirstDay)
        {
            break;
        }
        ss << "    "; 
    }
    for (auto [day_num, day_of_week] = pair{ 1, kFirstDay };
        day_num <= kNumberOfDays;
        ++day_num, MoveToNextDay(day_of_week))
    {
        ss << std::right << std::setfill(' ') << std::setw(3) << day_num << ' ';
        if (DaysOfWeek::kSunday == day_of_week)
        {
            ss << '\n';
        }
    }
    cout << ss.str();
}

int main()
{
    PrintMonthlyCalenday(Month::kDec, 2017);
    
    return 0;
}