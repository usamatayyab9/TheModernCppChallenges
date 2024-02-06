/**
 * @file main.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compiled with Qt 5.11.3 with gcc 9.4.0
 * 
 * This file is solution to "Problem 84. EAN-13 barcode generator"
 *  mentioned in "Chapter 10 : Archives, Images, and Databases" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * - A class `EAN_13Number` representing a valid EAN-13 number. See class comments for more details.
 * - A class `EAN13BarcodeGenerator` for genrating barcode image of a an `EAN_13Number` number. This
 *   class overload the function call operator for the user it to generate the barcode image. The
 *   EAN-13 number is provided as an argument to the function call opeator. See class comments for more details.
 * - A MainWindow class which implements the UI and uses an object of `EAN13BarcodeGenerator` to generate
 *   the barcode image and display it.
 * 
 * Driver code:
 * This program can operator in two modes. UI mode and CLI mode.
 * - UI
 *   To launch UI simply run the program without any argument. User can enter the number for which barcode
 *   is required. If the EAN-13 number is valid then the barcode is generated and displayed to user.
 *   Otherwise displays an error prompt.
 * - CLI mode:
 *   To run the application in CLI mode program expects only one additional argument, this argument
 *   is supposed to be a EAN-13 number. It this argument is a valid EAN-13 number then barcode image
 *   is saved in a png file and name of the file is same as the EAN-13 number.
 *   Providing more than one additional argument will cause the program to print a error message and
 *   terminate afterwards. 
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (1 == argc)
    {
        MainWindow w;
        w.show();
        return a.exec();
    }
    else if (2 == argc)
    {
        try {
            auto ean_13_number = EAN_13Number(argv[1]);
            qDebug() << "EAN-13 :" << ean_13_number.ToString().c_str();
            auto barcode_generator = EAN13BarcodeGenerator{};
            auto image = barcode_generator(ean_13_number);
            image.save(QString::fromStdString(ean_13_number.ToString()) + ".png");

        }  catch (const std::exception &e) {
            qDebug() << e.what();
        }
    }
    else
    {
        qDebug() << "Invalid number of arguments!!!";
    }
    return 0;
}
