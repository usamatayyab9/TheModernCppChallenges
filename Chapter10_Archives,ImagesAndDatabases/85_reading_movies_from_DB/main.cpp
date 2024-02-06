/**
 * @file main.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief
 * Compiled with Qt 5.11.3 with gcc 9.4.0
 * This file is solution to "Problem 85. Reading movies from an SQLite database"
 *  mentioned in "Chapter 10 : Archives, Images, and Databases" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * For SQL related I used Qt qsql functionalities.
 * For purpose of this problem an db file named 85_86_movies.db is used.
 * On startup application read the database file and populate the movies
 * list. Afterwards user can select any movie from the list and see its
 * details.
 * 
 * 
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
