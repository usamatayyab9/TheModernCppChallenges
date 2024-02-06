/**
 * @file main.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compiled with Qt 5.11.3 with gcc 9.4.0
 * 
 * This file is solution to "Problem 83. Creating verification text PNG images"
 *  mentioned in "Chapter 10 : Archives, Images, and Databases" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * A class `CaptchaGenerator` is provided for genrating random captchas. It ovarloads the
 * function call operator for generating the captcha image. All constants for defined in
 * file captchgenerator.h.
 * 
 * Driver code:
 * - This is an UI application.
 * - A UI consisting of label and button is displayed to.
 * - When user clicks Generate Captch button label is update to
 *      the captch generated.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
