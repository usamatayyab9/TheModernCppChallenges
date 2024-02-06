/**
 * @file    main.cpp
 * @author  Usama Tayyab (usamatayyab9@gmail.com)
 * @brief
 * Compiled with Qt 5.11.3 with gcc 9.4.0
 * 
 *  This file is solution to "Problem 91. Validating user credentials"
 *  mentioned in "Chapter 11 : Cryptography" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * This program takes two input from user 1) username and 2) password.
 * It then calculates hash of password entered by user. Then searches
 * the DB(91_user_credentials.db) for username and password hash. If the user is found
 * prints "Login successfull" to the console otherwise prints "Unable to find user".
 * For calculating hash crypto++ is used which is an off the shelf library and not work
 * of my own. For SQL related functionality Qt SQL facilities are used.
 *
 * NOTE: At time of writing this program DB contains only 4 users and their password is same
 *  as username
 *
 * 
 * 
 * @copyright Copyright (c) 2023
 * 
 */
//QT-includes
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QString>

//C++ includes
#include <iostream>
#include <string>
#include <string_view>

//Crypto++ includes
#include "cryptopp/sha.h"
#include "cryptopp/filters.h"
#include "cryptopp/base64.h"
#include "cryptopp/hex.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::string_view;

inline constexpr auto kUserCredentialsDBFile = string_view{ "../91_user_credentials.db" };

/**
 * @brief Calculate SHA256 digest of input string
 * 
 * @param password 
 * @return string 
 */
string CalculateDigest(string_view password)
{
    auto digest = string{};
    auto hash   = CryptoPP::SHA256{};
    CryptoPP::StringSource foo(password.data(), true,
        new CryptoPP::HashFilter(hash,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(digest)
            )
        )
    );
    return digest;
}

/**
 * @brief Tests whether the @param username and @param password exist in database or not.
 * @details
 * - Open database using Qt sql functionalities
 * - Calculate digest of param password string
 * - Prepare sql query for searching the specified username and password digest
 * - If the result of query is valid i.e. a record is found, then sets flag to true
 * - otherwise sets flag to false.
 * @param username 
 * @param password 
 * @return true  - if username and password are registered in database
 * @return false - otherwise
 */
bool DoesUserExist(string_view username, string_view password)
{
    auto is_user_found = false;
    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(kUserCredentialsDBFile.data());
    if (!db.open()) { cout << "Unable to open DB!!!" << endl; }
    else
    {
        auto password_hash = CalculateDigest(password);
        auto query = QSqlQuery{};
        query.prepare("SELECT id,username,first_name,last_name FROM user_credentials where username=? AND password=?");
        query.addBindValue(QVariant::fromValue(QString{ username.data() }));
        query.addBindValue(QVariant::fromValue(QString::fromStdString(password_hash)));
        if (query.exec() && query.first())
        {
            cout << query.value("id").toInt() << endl;
            cout << query.value("username").toString().toStdString() << endl;
            cout << query.value("first_name").toString().toStdString() << endl;
            cout << query.value("last_name").toString().toStdString() << endl;
            is_user_found = true;
        }
    }
    return is_user_found;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    auto username = string{};
    auto password = string{};
    cout << "Enter username: ";
    std::getline(cin, username);

    cout << "Enter password: ";
    std::getline(cin, password);

    if (DoesUserExist(username, password))
    {
        cout << "Login successfull" << endl;
    }
    else
    {
        cout << "Unable to find user" << endl;
    }

    return 0;
}
