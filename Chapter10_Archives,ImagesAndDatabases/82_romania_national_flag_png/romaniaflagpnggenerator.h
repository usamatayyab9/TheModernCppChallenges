/**
 * @file romaniaflagpnggenerator.h
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * This file contains class declaration of RomaniaFlagPNGGenerator
 * which is used to generate image of Romania flag.
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef ROMANIAFLAGPNGGENERATOR_H
#define ROMANIAFLAGPNGGENERATOR_H

#include <QImage>
#include <QString>
#include <QDebug>
#include <QPainter>
#include <QPen>

#include <cmath>


class RomaniaFlagPNGGenerator
{
public:
    RomaniaFlagPNGGenerator();
    QImage operator()(const int &width, const int &height, const QString &str);
};

#endif // ROMANIAFLAGPNGGENERATOR_H
