/**
 * @file main.cpp
 * @author Usama Tayyab (usamatayyab@gmail.com)
 * @brief 
 * Compiled with Qt 5.11.3 with gcc 9.4.0
 * 
 * This file is solution to "Problem 99. Translating text to any language"
 * mentioned in "Chapter 12 : Networking and Services" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * For solving problem I used the google API and Qt nerworking resources. Following two function are defined for this
 * solution:
 * - `GetJsonForQuery()` : Which retunrs json string which will be added as payload in json request.
 * - `GetTranslatedTextFromResponse()` After making the request, once the response is received this function is
 *    called with the received bytes. It is responsible for parsing the response string.  
 * 
 * Driver code:
 * - Initializes the text string which has to be translate.
 * - Initalizes Api URL. Replace with API_KEY your api key.
 * - Make requests for translation.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <QCoreApplication>
#include <QString>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QByteArray>
#include <QStringLiteral>
#include <QObject>
#include <QThread>

//C++ includes
#include <iostream>
#include <vector>
#include <tuple>

/**
 * @brief Get the Json For Query object
 * 
 * @param kText 
 * @param kFrom 
 * @param kTo 
 * @return QByteArray 
 */
QByteArray GetJsonForQuery(const QString &kText, const QString &kFrom, const QString &kTo)
{
    auto json_str = QJsonObject{};
    json_str["source"]  = kFrom;
    json_str["q"]       = kText;
    json_str["target"]  = kTo;
    return QJsonDocument{ json_str }.toJson();
}

/**
 * @brief Get the Translated Text From Response object
 * 
 * @param response_json 
 * @return auto 
 */
auto GetTranslatedTextFromResponse(const QString &response_json)
{
    auto translated_text = std::wstring{};
    auto json_doc        = QJsonDocument::fromJson(response_json.toUtf8());
    if ((false == json_doc.isNull()) && json_doc.isObject())
    {
        const auto kJsonObj           = json_doc.object();
        const auto kDataObj           = kJsonObj["data"].toObject();
        const auto kTranslationsArray = kDataObj["translations"].toArray();
        const auto kkTranslatedObj    = kTranslationsArray[0].toObject();
        translated_text               = kkTranslatedObj["translatedText"].toString("").toStdWString();
    }
    return translated_text;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    const auto kTexts = std::vector<std::tuple<std::wstring, QString, QString>>
    {
        { L"This is my first translation"     , "en", "ur" },
        { L"Dies ist meine zweite Übersetzung", "de", "ur" },
        { L"یہ انگریزی میں متن ہے۔"           , "ur", "en" },
        { L"Bonjour le monde"                 , "fr", "en" },
        { L"How are you?"                      , "en", "hi" }
    };
    constexpr auto kApi  = std::string_view{ "https://translation.googleapis.com/language/translate/v2?key="
                                             "API_KEY" };
    auto network_manager = QNetworkAccessManager{};
    auto request         = QNetworkRequest{ QUrl{ kApi.data() } };
    request.setHeader(QNetworkRequest::ContentTypeHeader, "Content-Type: application/json; charset=utf-8");

    for (const auto &[kText, kFrom, kTo] : kTexts)
    {
        auto payload        = GetJsonForQuery(QString::fromStdWString(kText), kFrom, kTo);
        auto *network_reply = network_manager.post(request, payload);
        QObject::connect(network_reply, &QNetworkReply::finished, [=]()
        {
            const auto kTranslatedText = QString::fromStdWString(GetTranslatedTextFromResponse(network_reply->readAll()));
            qDebug() << kFrom << " : " << QString::fromStdWString(kText);
            qDebug() << kTo << " : " << kTranslatedText << "\n";

            network_reply->deleteLater();
        });
    }

    return a.exec();
}
