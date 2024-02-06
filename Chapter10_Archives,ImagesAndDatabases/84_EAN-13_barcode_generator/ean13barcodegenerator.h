/**
 * @file ean13barcodegenerator.h
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * This file declares the class for geenrating a barcode image of an EAN-13 number.
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef EAN13BARCODEGENERATOR_H
#define EAN13BARCODEGENERATOR_H

#include <QImage>
#include <QPainter>
#include <QString>
#include <QDebug>

#include <array>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include <numeric>

#include "ean_13_number.h"

std::unordered_map<int, std::unordered_map<char, std::string>> GetDigitEncodings();

/**
 * @brief Class for genrating barcode image
 * https://en.wikipedia.org/wiki/International_Article_Number#Binary_encoding_of_data_digits_into_EAN-13_barcode
 * Before understanding this code please visit above link and get understanding of EAN-13 numbers and
 * how their barcode is generated
 */
class EAN13BarcodeGenerator
{
    
    static constexpr auto kSubGroupEncodings = std::array<std::pair<std::string_view, std::string_view>, 10>
    {{
        { "LLLLLL", "RRRRRR" },
        { "LLGLGG", "RRRRRR" },
        { "LLGGLG", "RRRRRR" },
        { "LLGGGL", "RRRRRR" },
        { "LGLLGG", "RRRRRR" },
        { "LGGLLG", "RRRRRR" },
        { "LGGGLL", "RRRRRR" },
        { "LGLGLG", "RRRRRR" },
        { "LGLGGL", "RRRRRR" },
        { "LGGLGL", "RRRRRR" }
    }};
    static const std::unordered_map<int, std::unordered_map<char, std::string>> kDigitEncodings;
    static constexpr auto kTotalBars          = 95;
    static constexpr auto kBarWidth           = 2;
    static constexpr auto kBarHeight          = 87;
    static constexpr auto kQuietZoneSize      = 30;
    static constexpr auto kImageWidth         = (kTotalBars * kBarWidth) + (2 * kQuietZoneSize); //leave empty space on right and left
    static constexpr auto kImageHeight        = kBarHeight + (2 * 10); // leave 10 units above and below
    static constexpr auto kSubGroupStringSize = 42;
public:
    EAN13BarcodeGenerator() = default;
    QImage operator()(const EAN_13Number &number);
    std::string GetFinalEncoding(const EAN_13Number &number);
    std::string GetSubGroupEncoding(std::string_view kDigits, std::string_view kSubGroupEncoding);
};

#endif // EAN13BARCODEGENERATOR_H
