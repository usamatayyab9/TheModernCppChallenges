#include "ean13barcodegenerator.h"

/**
 * @brief Initializes the static member variable kDigitEncodings with the digit encodings required for barcode generation.
 */
decltype (EAN13BarcodeGenerator::kDigitEncodings) EAN13BarcodeGenerator::kDigitEncodings = GetDigitEncodings();

/**
 * @brief Generates an EAN-13 barcode image for the given EAN-13 number.
 * 
 * @param ean_number The EAN-13 number for which the barcode image is generated.
 * @return QImage The generated barcode image.
 */
QImage EAN13BarcodeGenerator::operator()(const EAN_13Number &ean_number)
{
    auto barcode_image = QImage(kImageWidth, kImageHeight, QImage::Format_RGB32);
    barcode_image.fill(Qt::white);
    auto painter = QPainter(&barcode_image);
    const auto kFinalEncoding = GetFinalEncoding(ean_number);
    qDebug() << "Final encoding: " << kFinalEncoding.c_str() << ", size of encodings: " << kFinalEncoding.size();
    //safe check just to be sure
    if (kTotalBars == kFinalEncoding.size())
    {
        auto x_coord = kQuietZoneSize;
        for (const auto &kCh : kFinalEncoding)
        {
            if      ('1' == kCh) { painter.setBrush(QBrush(Qt::black, Qt::SolidPattern)); painter.setPen(QPen(Qt::black)); }
            else if ('0' == kCh) { painter.setBrush(QBrush(Qt::white, Qt::SolidPattern)); painter.setPen(QPen(Qt::white)); }
            painter.drawRect(x_coord, 10, kBarWidth, kBarHeight);
            x_coord += kBarWidth;
        }
    }
    return barcode_image;
}

/**
 * @brief Gets the final encoding for the given EAN-13 number.
 * 
 * @param number The EAN-13 number for which the encoding is generated.
 * @return std::string The final encoding string.
 */
std::string EAN13BarcodeGenerator::GetFinalEncoding(const EAN_13Number &number)
{
    const auto kEANStr      = number.ToString();
    const auto kFirstDigit  = kEANStr.front() - '0';
    const auto kLeftDigits  = kEANStr.substr(1, 6);
    const auto kRightDigits = kEANStr.substr(7);
    const auto [kLeftGroupEncoding, kRightGroupEncoding] = kSubGroupEncodings[static_cast<size_t>(kFirstDigit)];
    auto final_encodings = std::string{};
    final_encodings.append("101"); // Start Marker
    final_encodings.append(GetSubGroupEncoding(kLeftDigits, kLeftGroupEncoding));
    final_encodings.append("01010");
    final_encodings.append(GetSubGroupEncoding(kRightDigits, kRightGroupEncoding));
    final_encodings += "101"; // End marker
    //size(final_encodings) == 95
    return final_encodings;
}

/**
 * @brief Gets the subgroup encoding for the given set of digits and subgroup encoding pattern.
 * 
 * @param kDigits The set of digits.
 * @param kSubGroupEncoding The subgroup encoding pattern.
 * @return std::string The subgroup encoding string.
 */
std::string EAN13BarcodeGenerator::GetSubGroupEncoding(std::string_view kDigits, std::string_view kSubGroupEncoding)
{
//    auto sub_group_encoding = std::string{};
//    for (auto idx = size_t{ 0 }; idx < kDigits.size(); ++idx)
//    {
//        const auto kDigit    = kDigits[idx] - '0';
//        const auto kCode     = kSubGroupEncoding[idx];
//        const auto kEncoding = kDigitEncodings.at(kDigit).at(kCode);
//        sub_group_encoding += kEncoding;
//    }
    //Below code is eqivalent to above
    /** 42 areas for each group i.e. 7 areas per digit.
     * It is responsibility of the caller to pass 6 digits in param kDigits.
    */
    auto sub_group_encoding = std::string(kSubGroupStringSize, '\0');
    std::inner_product(
        kDigits.cbegin(),
        kDigits.cend(),
        kSubGroupEncoding.cbegin(),
        sub_group_encoding.begin(),
        [](auto sub_group_encoding_iter, const std::string &kDigitEncoding) {
            auto pos = std::copy(kDigitEncoding.cbegin(), kDigitEncoding.cend(), sub_group_encoding_iter);
            return pos;
        },
        [](const auto &kDigit, const auto &kCode) {
            return kDigitEncodings.at(kDigit - '0').at(kCode);
        }
    );
    return sub_group_encoding;
}


std::unordered_map<int, std::unordered_map<char, std::string>> GetDigitEncodings()
{
    auto digit_encodings = std::unordered_map<int, std::unordered_map<char, std::string>>{};
    digit_encodings[0] = std::unordered_map<char, std::string>{ {'L', "0001101"}, { 'G', "0100111"}, { 'R', "1110010"} };
    digit_encodings[1] = std::unordered_map<char, std::string>{ {'L', "0011001"}, { 'G', "0110011"}, { 'R', "1100110"} };
    digit_encodings[2] = std::unordered_map<char, std::string>{ {'L', "0010011"}, { 'G', "0011011"}, { 'R', "1101100"} };
    digit_encodings[3] = std::unordered_map<char, std::string>{ {'L', "0111101"}, { 'G', "0100001"}, { 'R', "1000010"} };
    digit_encodings[4] = std::unordered_map<char, std::string>{ {'L', "0100011"}, { 'G', "0011101"}, { 'R', "1011100"} };
    digit_encodings[5] = std::unordered_map<char, std::string>{ {'L', "0110001"}, { 'G', "0111001"}, { 'R', "1001110"} };
    digit_encodings[6] = std::unordered_map<char, std::string>{ {'L', "0101111"}, { 'G', "0000101"}, { 'R', "1010000"} };
    digit_encodings[7] = std::unordered_map<char, std::string>{ {'L', "0111011"}, { 'G', "0010001"}, { 'R', "1000100"} };
    digit_encodings[8] = std::unordered_map<char, std::string>{ {'L', "0110111"}, { 'G', "0001001"}, { 'R', "1001000"} };
    digit_encodings[9] = std::unordered_map<char, std::string>{ {'L', "0001011"}, { 'G', "0010111"}, { 'R', "1110100"} };
    return digit_encodings;
}
