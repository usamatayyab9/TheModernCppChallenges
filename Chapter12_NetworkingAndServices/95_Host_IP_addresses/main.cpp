/**
 * @file main.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compiled with Qt 5.11.3 with gcc 9.4.0
 * 
 * This file is solution to "Problem 95. Finding the IP address of a host"
 * mentioned in "Chapter 12 : Networking and Services" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * For solving this problem I used Qt. Because it provides good documentation, I am more familiart with
 * Qt and it is cross platform. `QHostInfo` class provides necessary methods for solving problem.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <QCoreApplication>
#include <QHostInfo>
#include <QHostAddress>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    auto host_info = QHostInfo::fromName("google.com");
    foreach (const auto &host_address, host_info.addresses())
    {
        qDebug() << host_address.toString();
    }
    return 0;
}
