/**
 * @file main.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compiled with Qt 5.11.3 with gcc 9.4.0
 * 
 * This file is solution to "Problem 78. Creating a PDF from a collection of images"
 *  mentioned in "Chapter 9 : Data Serialization" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * To solve this problem I once again used the Qt functionalities. When the program
 * is launched A user interface is shown in which user can select the directory
 * which should contains all images. The program traverses the directory and
 * generates the pdf file which contains all images found in the directory.
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
