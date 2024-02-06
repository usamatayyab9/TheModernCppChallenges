/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * Compiled with Qt 5.11.3 with gcc 9.4.0
 * 
 *  This file is solution to "Problem 87. Handling movie images in an SQLite database"
 *  mentioned in "Chapter 10 : Archives, Images, and Databases" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * For this problem "87_movies_media.db" is used. It has an extra table for storing
 * media objects. The program allows user to search movie by id and title.
 * Once the user searched the movie it can select the media files which user want
 * to upload for the selected movie. Similarly user can also delete the media files
 * from db.
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
