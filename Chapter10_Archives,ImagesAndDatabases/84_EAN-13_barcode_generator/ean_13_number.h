/**
 * @file ean_13_number.h
 * @author Usama Tayyab (usamtayyab9@gmail.com)
 * @brief
 * 
 * This file defines the class `EAN_13Number` for representing a EAN-13 number/.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef EAN_13_NUMBER_H
#define EAN_13_NUMBER_H

#include <iostream>
#include <string_view>
#include <string>
#include <array>
#include <numeric>
#include <exception>
#include <algorithm>
#include <locale>

/**
 * @brief EAN_13Number class represents a 13-digit EAN-13 number with a check digit.
 * The class provides methods for validating and initializing EAN-13 numbers.
 */
class EAN_13Number
{
public:
    /**
     * @brief Default constructor is deleted to enforce initialization through valid EAN-13 digits.
     */
    EAN_13Number() = delete;
    /**
     * @brief Parameterized constructor to initialize an EAN-13 number from a string view.
     * @param ean_13_view The string view containing the EAN-13 number digits.
     * @throws std::invalid_argument If the provided string view is not a valid EAN-13 number.
     */
    EAN_13Number(std::string_view ean_13_view)
    {
        auto is_valid = false;
        //For valid EAN-13 number size must be 12 or 13 and all characters must be digits
        if (const auto kSz = std::size(ean_13_view); (13 == kSz || 12 == kSz) && AreAllDigits(ean_13_view))
        {
            const auto kCheckSumDigit = GetCheckSumDigit(ean_13_view.substr(0, 12));
            std::cout << "Checksum: " << kCheckSumDigit << "\n";
            is_valid = (12 == kSz) || (kCheckSumDigit == ean_13_view[12]); 
            if (12 == kSz)
            {
                number_[12] = kCheckSumDigit;
            }
        }
        
        if (is_valid)
        {
            std::copy(std::cbegin(ean_13_view), std::cend(ean_13_view), std::begin(number_));
        }
        else
        {
            throw std::invalid_argument("Invalid EAN-13 digit number provided");
        }
    }
    EAN_13Number(const unsigned int &ean_13_num) : EAN_13Number(std::to_string(ean_13_num))
    {
    }

    std::string ToString() const
    {
        return { std::cbegin(number_), std::cend(number_) };
    }
private:
    bool AreAllDigits(std::string_view code)
    {
        return std::all_of(std::cbegin(code), std::cend(code), [loc = std::locale{}](const auto &ch){
            return std::isdigit(ch, loc);
        });
    }

    /**
     * @brief Calculates and returns the check digit for the EAN-13 number.
     * @param code The string view representing the first 12 digits of the EAN-13 number.
     * @return The calculated check digit.
     */
    char GetCheckSumDigit(std::string_view code)
    {
        auto flag = true;
        auto kWeightedSum = std::accumulate(code.cbegin(), code.cend(), 0,
        [&flag](const auto &init, const auto &digit)
        {
            auto wt = 0;
            if (flag) { wt = 1; }
            else      { wt = 3; }
            flag = !flag;
            return init + wt * (digit - '0');
        });
        const auto kMod = kWeightedSum % 10;
        return '0' + (0 == kMod ? kMod : 10 - kMod);
    }
    std::array<char, 13> number_;
};

#endif // EAN_13_NUMBER_H
