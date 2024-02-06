/**
 * @file date_utility_functions.h
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * This file provides all the utility function for performing
 * date calculations such testing leap year, getting number of
 * days in a month or between two dates, converting days of week
 * to string, calculating week number of a date, calculating
 * total weeks in a year etc. See each function comments for 
 * details.
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef DATE_UTILITY_FUNCTIONS_H
#define DATE_UTILITY_FUNCTIONS_H

#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>

#include "date.h"

using std::cout;
using std::endl;
using std::floor;
using std::string;

static string ToString(const DaysOfWeek &week_day)
{
    auto str = string{};
    switch (week_day)
    {
        case DaysOfWeek::kMonday    : { str = "Monday";    break; }
        case DaysOfWeek::kTuesday   : { str = "Tuesday";   break; }
        case DaysOfWeek::kWednesday : { str = "Wednesday"; break; }
        case DaysOfWeek::kThursday  : { str = "Thursday";  break; }
        case DaysOfWeek::kFriday    : { str = "Friday";    break; }
        case DaysOfWeek::kSaturday  : { str = "Saturday";  break; }
        case DaysOfWeek::kSunday    : { str = "Sunday";    break; }
    }
    return str;
}

/**
 * @brief Checks if a given year is a leap year.
 * 
 * This function determines whether a specified year is a leap year based on the rules
 * of the Gregorian calendar. A leap year occurs every 4 years, except for years that are
 * divisible by 100 but not divisible by 400, which are not leap years.
 * 
 * @param year The year to be checked for leap year status.
 * @return true if the year is a leap year, false otherwise.
 */
static constexpr bool IsLeapYear(const int &year)
{
    return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}

/**
 * @brief Calculates the number of days in a given month for a specified year.
 * 
 * This function determines the number of days in a given month based on the provided
 * Month enumeration and year. It takes into account leap years for February.
 * 
 * @param month The Month enumeration representing the month.
 * @param year The year for which to calculate the number of days.
 * @return The number of days in the specified month for the given year.
 */
static constexpr size_t DaysInMonth(const Month &month,  const int &year)
{
    auto d = 0;
    switch (month)
    {
        case (Month::kJan)       : { d = 31; break; }
        case (Month::kFeb)       : { d = IsLeapYear(year) ? 29 : 28; break; }
        case (Month::kMarch)     : { d = 31; break; }
        case (Month::kApril)     : { d = 30; break; }
        case (Month::kMay)       : { d = 31; break; }
        case (Month::kJune)      : { d = 30; break; }
        case (Month::kJuly)      : { d = 31; break; }
        case (Month::kAug)       : { d = 31; break; }
        case (Month::kSept)      : { d = 30; break; }
        case (Month::kOct)       : { d = 31; break; }
        case (Month::kNov)       : { d = 30; break; }
        case (Month::kDec)       : { d = 31; break; }
        default                  : { break; }
    }
    return d;
}

/**
 * @brief Calculates the number of days since the start of the year until the specified date.
 * 
 * This function computes the total number of days since the beginning of the year until the
 * specified date, taking into account the days in each preceding month. It uses the provided
 * Date structure, which includes information about the day, month, and year.
 * 
 * @param date The Date structure representing the specific date.
 * @return The number of days since the start of the year until the specified date.
 */
static constexpr size_t DaysSinceStartOfYear(const Date &date)
{
    auto no_of_days_since_start_of_year = date.day;
    for (const auto &month : kMonthsList)
    {
        if (month == date.month) { break; }
        else
        {
            no_of_days_since_start_of_year += DaysInMonth(month, date.year);
        }
    }
    return no_of_days_since_start_of_year;
}

/**
 * @brief Counts the number of leap years between two given years (inclusive).
 * 
 * This function calculates the count of leap years between two specified years, including the years
 * provided in the range. It utilizes the IsLeapYear function to determine if a given year is a leap year.
 * The order in which years are specified does not matter
 * 
 * @param year1 The first year in the range.
 * @param year2 The second year in the range.
 * @return The count of leap years between year1 and year2 (inclusive).
 */
static size_t LeapYearsBetween(const int &year1, const int &year2)
{
    const auto [kMinYear, kMaxYear] = std::minmax(year1, year2);
    auto leap_year_count            = size_t{ 0 };

    for (auto first = kMinYear, last = kMaxYear; first < last ;++first)
    {
        leap_year_count += IsLeapYear(first);
    }
    return leap_year_count;
}

/**
 * @brief Calculates the number of days between two given dates.
 * 
 * This function determines the total number of days between two specified dates, taking into account
 * the leap years, common years, and days within each year. It uses helper functions such as
 * DaysSinceStartOfYear and LeapYearsBetween for the calculations.
 * 
 * @param d1 The first date.
 * @param d2 The second date.
 * @return The count of days between d1 and d2 (inclusive).
 * NOTE: Cannot be made constexpr because abs is not constexpr.
 */
static size_t DaysBetweenDates(const Date &d1, const Date &d2)
{
    const auto kDaysBeforeD1        = DaysSinceStartOfYear(d1);
    const auto kDaysBeforeD2        = DaysSinceStartOfYear(d2);
    const auto kLeapYears           = LeapYearsBetween(d1.year, d2.year);
    const auto kTotalYears          = std::abs(d1.year - d2.year);
    const auto kCommonYears         = kTotalYears - kLeapYears;
    const auto kTotalDays           = (kCommonYears * 365) + (kLeapYears * 366) - kDaysBeforeD1 + kDaysBeforeD2;
    return kTotalDays;
}

/**
 * @brief Calculates the day of the week for a given date.
 * 
 * This function determines the day of the week for a specified date based on the comparison
 * with the epoch date (January 1, 1970, which is a Thursday). It uses the DaysBetweenDates function
 * to calculate the number of days between the epoch date and the input date and then calculates
 * the day of the week accordingly.
 * 
 * @param date The input date.
 * @return The day of the week for the given date (as an enum value of DaysOfWeek).
 * 
 * @note The function assumes the epoch date is Thursday (DaysOfWeek::kThursday).
 * @note The function is constexpr, but it relies on DaysBetweenDates, which is not constexpr due to the use of abs.
 */
static constexpr DaysOfWeek WeekDayOf(const Date &date)
{
    constexpr auto kEpochDate    = Date{ 1, Month::kJan, 1970 };
    constexpr auto kEpochWeekDay = static_cast<int>(DaysOfWeek::kThursday);
    auto week_day                = kEpochWeekDay;
    auto date_comparison_result  = kEpochDate.compare(date);

    if (-1 == date_comparison_result) { 
        //kEpochDate is before date
        auto days = DaysBetweenDates(kEpochDate, date);
        week_day  = ((kEpochWeekDay + days) % 7);
    }
    else if (1 == date_comparison_result) { 
        //kEpochDate is after date
        auto days = DaysBetweenDates(date, kEpochDate);
        week_day  = ((kEpochWeekDay - static_cast<int>(days)) % 7) + 7;
    }
    return static_cast<DaysOfWeek>(week_day);
}

/**
 * @brief Calculates the number of ISO weeks in a given year.
 * 
 * This function determines the number of ISO weeks in a specified year based on ISO week date rules.
 * ISO weeks start from Monday and each week has 4 or 5 days. The function checks if the last day of the
 * year and the last day of the previous year correspond to certain weekdays, and if so, adds an extra week
 * to the total count.
 * 
 * @param year The year for which to calculate the number of weeks.
 * @return The number of ISO weeks in the given year.
 * 
 * @note The function is constexpr and relies on the WeekDayOf function.
 * @see https://en.wikipedia.org/wiki/ISO_week_date
 */
static constexpr auto WeekCount(const int &year)
{
    auto weeks_in_year = 52;
    if (WeekDayOf(Date{ 31, Month::kDec, year }) == DaysOfWeek::kThursday
        || WeekDayOf(Date{ 31, Month::kDec, year - 1 }) == DaysOfWeek::kWednesday)
    {
        ++weeks_in_year; // 53 weeks
    }
    return weeks_in_year;
}

/**
 * @brief Calculates the ISO week number for a given date.
 * 
 * This function uses the ISO week date algorithm to determine the week number of a specified date.
 * ISO weeks start from Monday, and each week has 4 or 5 days. The algorithm considers the day of the year,
 * the day of the week, and the total number of ISO weeks in the year to calculate the week number.
 * 
 * @param d The date for which to calculate the ISO week number.
 * @return The ISO week number for the given date.
 * 
 * @note Algorithm taken from https://en.wikipedia.org/wiki/ISO_week_date
 * This implemnetation is buggy for Date{ 25, Month::kSept, 1607 } function should return
 * 38 but it returns 39
 */
static int WeekNum(const Date &d)
{
    const auto kYear            = d.year;
    const auto kDayOfYear       = DaysSinceStartOfYear(d); // day in ordinal date(DDD/YYYY)
    const auto kDayOfWeek       = static_cast<int>(WeekDayOf(d)) + 1;
    const auto kW               = std::floor( (10.0f + kDayOfYear - kDayOfWeek) / 7.0f );
    const auto kNoOfWeeksInYear = WeekCount(kYear);
    auto week_num = -1;
   
    if (kW < 1)                     { week_num = WeekCount(kYear - 1); }
    else if (kW > kNoOfWeeksInYear) { week_num = 1;                    }
    else                            { week_num = kW;                   }

   return week_num;
}

#endif //DATE_UTILITY_FUNCTIONS_H