#include "romaniaflagpnggenerator.h"

RomaniaFlagPNGGenerator::RomaniaFlagPNGGenerator()
{

}

/**
 * @brief Generates the Romania flag image with given dimensions and store in a file
 * with name param str.
 * 
 * @param width 
 * @param height 
 * @param str 
 * @return QImage 
 */
QImage RomaniaFlagPNGGenerator::operator()(const int &width, const int &height, const QString &str)
{
    auto flag_image     = QImage(width, height, QImage::Format_RGB32);
    auto painter        = QPainter(&flag_image);
    const auto kSplit   = std::ceil(width / 3.0f);
    const auto kSplit2  = std::floor(width / 3.0f);

    painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
    painter.setPen(QPen(Qt::red));
    painter.drawRect(QRect(0, 0, kSplit, height));

    painter.setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
    painter.setPen(QPen(Qt::yellow));
    painter.drawRect(QRect(kSplit, 0, kSplit, height));

    painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
    painter.setPen(QPen(Qt::blue));
    painter.drawRect(QRect(2 * kSplit, 0, kSplit2, height));

    if (false == flag_image.save(str)) {
        qDebug() << "File not saved successfully";
        flag_image = QImage{}; // set to null image
    }
    return flag_image;
}
