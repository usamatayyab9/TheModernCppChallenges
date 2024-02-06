/**
 * @file main.cpp
 * @author Usama Tayyab (usamtayyab9@gmail.com)
 * @brief 
 * 
 * Compiled with Qt 5.11.3 with gcc 9.4.0
 * 
 * This file is solution to "Problem 97. Bitcoin exchange rates"
 * mentioned in "Chapter 12 : Networking and Services" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QObject>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QJsonParseError>

/**
 * @brief Prints Bitcoin exchange rates from JSON data.
 *
 * This function takes a QByteArray containing JSON data representing Bitcoin exchange rates.
 * It parses the JSON data, extracts information about the exchange rates, and prints the
 * exchange rate of 1 Bitcoin for each currency code found in the JSON object.
 *
 * @param data The QByteArray containing JSON data.
*/
void PrintBitcointExchangeRates(const QByteArray &data)
{
    qDebug() << "Printing Bitcoin exchange rates: " << data;
    auto parse_error = QJsonParseError{};
    auto json_doc = QJsonDocument::fromJson(data, &parse_error);
    if (json_doc.isNull()) { qDebug() << parse_error.errorString(); }
    else
    {
        auto json_obj = json_doc.object();
        qDebug() << json_obj.size();
        for (const auto &kKey : json_obj.keys())
        {
            //kKey will be currency code such as USD,INR,PKR,EUR,GBR etc.
            auto currency_exhcnage_info = json_obj[kKey].toObject();
            qDebug() << "1 BTC = " << currency_exhcnage_info["last"].toDouble() << kKey;
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    auto network_manager = QNetworkAccessManager{};
    auto request         = QNetworkRequest{ QUrl{ "https://blockchain.info/ticker" } };

    QObject::connect(&network_manager, &QNetworkAccessManager::finished, [](QNetworkReply *reply)
    {
        emit PrintBitcointExchangeRates(reply->readAll());
    });
    network_manager.get(request);
    return a.exec();
}
