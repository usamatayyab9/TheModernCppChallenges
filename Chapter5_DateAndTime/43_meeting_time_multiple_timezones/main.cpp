/**
 * @file main.cpp
 * @author Usama Tayyab (usamtayyab9@gmail.com)
 * @brief 
 * This code was compile on Qt 5.11.3, gcc 9.4.0 and qmake version 3.1
 * 
 *  This file is solution to "Problem 43. Meeting time for multiple time zones"
 *  mentioned in "Chapter 5: Date and Time" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a function that, given a list of meeting participants and
 * their time zones, displays the local meeting time for each participant.
 * 
 * Solution:
 * For solving this problem invlovles working different timezones.
 * Implementing a solution from scratch reuqires deep understanding
 * of timezones. The purpose was to solve the problem not to develop
 * a library that solves the problem. So therefore I used an external
 * framework Qt.
 * 
 * There are many other libraries which provide functionality
 * for converting a time to different timezones. But I was
 * more familiar with Qt so I choose to go with Qt.
 * 
 * Driver code:
 * The program firsts defines a meeting time.
 * Then defines a list user with differetn name
 * and timezones
 * And finally for each user prints the meeting time in
 * their respective timezones
 *  
 * @copyright Copyright (c) 2023
 * 
 */
#include <QCoreApplication>
#include <QTimeZone>
#include <QDateTime>
#include <QDebug>
#include <QString>

#include <vector>

/**
 * @brief The User struct containing the name of user and its timezone.
 */
struct User
{
    QString name;
    QTimeZone time_zone;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    const auto kMeetingTime = QDateTime(QDate{ 2023, 12, 1 }, QTime{ 23, 58, 0 }, QTimeZone{ "Asia/Karachi" });
    const auto kUsers       = std::vector<User>{
        { "Javeria", QTimeZone{ "Europe/Berlin"    } },
        { "Usama"  , QTimeZone{ "Europe/London"    } },
        { "Ahmad"  , QTimeZone{ "Asia/Singapore"   } }
    };
    for (const auto &[kName, kTimeZone] : kUsers)
    {
        qDebug() << "Meeting time for " << kName << " is " << kMeetingTime.toTimeZone(kTimeZone).toString("dddd dd/MM/yyyy HH:mm:ss");
    }

    return a.exec();
}
