/**
 * @file main.cpp
 * @author Usama Tayyab (usamatayyab@gmail.com)
 * @brief 
 * Compiled with Qt 5.11.3 with gcc 9.4.0
 * 
 * This file is solution to "Problem 86. Inserting movies into an SQLite database transactionally"
 *  mentioned in "Chapter 10 : Archives, Images, and Databases" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * The program start by launching UI window. This contains all field for movie data.
 * Once the user enters all the field the info entered by the user is enetered into
 * db. For this program a db file "85_86_movies.db" is used. All records are inserted
 * into this db.
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
