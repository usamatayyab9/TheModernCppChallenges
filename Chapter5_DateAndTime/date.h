/**
 * @file date.h
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * This file defines enumerations for month, days and a Date struct.
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef DATE_H
#define DATE_H

#include <array>

using std::array;

enum class Month
{
    kJan,
    kFeb,
    kMarch,
    kApril,
    kMay,
    kJune,
    kJuly,
    kAug,
    kSept,
    kOct,
    kNov,
    kDec
};

inline constexpr auto kMonthsList = array{
    Month::kJan,
    Month::kFeb,
    Month::kMarch,
    Month::kApril,
    Month::kMay,
    Month::kJune,
    Month::kJuly,
    Month::kAug,
    Month::kSept,
    Month::kOct,
    Month::kNov,
    Month::kDec,
};

enum class DaysOfWeek {
    kMonday = 0,
    kTuesday,
    kWednesday,
    kThursday,
    kFriday,
    kSaturday,
    kSunday
};

inline constexpr auto kDaysList = array{
    DaysOfWeek::kMonday,
    DaysOfWeek::kTuesday,
    DaysOfWeek::kWednesday,
    DaysOfWeek::kThursday,
    DaysOfWeek::kFriday,
    DaysOfWeek::kSaturday,
    DaysOfWeek::kSunday
};

/**
 * @brief A structure for defining dates also provides a function for comparing dates.
 * 
 */
struct Date
{
    int day;
    Month month;
    int year;
    /**
     * @brief Compares two dates
     * 
     * @param rhs 
     * @return int 
     *          -1, if lhs occurs before rhs
     *          0, if lhs is the same date as rhs
     *          1, if lhs occurs after rhs
     */
    constexpr int compare(const Date &rhs) const
    {
        auto Compare = [](const int &a, const int &b){
            if      (a < b) { return -1; }
            else if (a > b) { return 1;  }
            return 0;
        };

        const auto &lhs = *this;
        auto result     = Compare(lhs.year, rhs.year);
        if (0 == result)
        {
            result = Compare(static_cast<int>(lhs.month), static_cast<int>(rhs.month));
            if (0 == result)
            {
                result = Compare(lhs.day, rhs.day);
            }
        }

        return result;
    }
};

#endif //DATE_H