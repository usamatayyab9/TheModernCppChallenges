/**
 * @file main.cpp
 * @author Usama Tayyab (usamatyyab9@gmail.com)
 * @brief 
 * Compiled with Qt 5.11.3 with gcc 9.4.0
 * 
 * This file is solution to "Problem 100. Detecting faces in a picture"
 * mentioned in "Chapter 12 : Networking and Services" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution: 
 * To solve this problem I used google cloud vision api and Qt for networking functionalities.
 * To send an image to as payload it has to be converted to base 64 encoding scheme and should added as 
 * json payload to request. Following functions are defined:
 * - `GetJsonPayloadForFaceDetectionApi()` for generating json request for API.
 * - `ReadFile()` read a file and returns all of its contents as string.
 * - `ReadFileToBase64()` reads a file and converts it into Base 64 encoding.
 * - `HandleResponse()` for parsing response and extracting required info from API.
 * 
 * Driver code:
 * This program expects one addition argument which should be path to picture file in which user
 * wants to detect faces.
 * - Generates the json payload
 * - Prepaes network request
 * - Accumulates all data that is received in chunks
 * - Once all data is received extract and prints the required info.
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

#include <fstream>
#include <string>
#include <iterator>
#include <iostream>

#include "base64_utility.h"

/**
 * @brief Get the Json Payload For Face Detection Api object
 * 
 * @param str 
 * @return string - JSON payload as string
 */
auto GetJsonPayloadForFaceDetectionApi(string_view str)
{
    return QString{
        "{\"requests\":[{\"features\":[{\"maxResults\":10,\"type\":\"FACE_DETECTION\"}],\"image\":{\"content\":\"%1\"}}]}"
    }.arg(str.data());
}

/**
 * @brief Reads the entire file and return as string
 * 
 * @param filepath 
 * @return auto 
 */
auto ReadFile(std::string_view filepath)
{
    auto fin = std::ifstream{ filepath.data() };
    return std::string{ std::istreambuf_iterator<char>{ fin }, std::istreambuf_iterator<char>{} };
}

/**
 * @brief Read the file and convert into Base 64 encoding
 * 
 * @param filepath 
 * @return auto 
 */
auto ReadFileToBase64(std::string_view filepath)
{
    return Base64::Encode(
        Base64::StringToBinaryData(
            ReadFile(
                filepath
            )
        )
    );
}

/**
 * @brief Handle response from cloud vision api.
 * 
 * @param response 
 */
void HandleResponse(const QByteArray &response)
{
    const auto kJsonDoc               = QJsonDocument::fromJson(response);
    const auto kJsonObj               = kJsonDoc.object();
    const auto kResponsesArray        = kJsonObj["responses"].toArray();
    const auto kFaceAnnotationsObject = kResponsesArray[0].toObject();
    const auto kFaceAnnotationsArray  = kFaceAnnotationsObject["faceAnnotations"].toArray();
    std::cout << "Total faces found: " << kFaceAnnotationsArray.size() << "\n";
    for (auto face_idx = 0; face_idx < kFaceAnnotationsArray.size(); ++face_idx)
    {
        const auto &kFaceData = kFaceAnnotationsArray[face_idx];
        const auto kRectPoints = kFaceData["boundingPoly"]["vertices"].toArray();
        std::cout << "Face " << face_idx << " points:\n";
        for (const auto &kPoints : kRectPoints)
        {
            std::cout << "\t" << "x : " << kPoints["x"].toInt() << ", y : " << kPoints["y"].toInt() << "\n";
        }
        std::cout << "\n";
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc < 2)
    {
        std::cout << "Please provide 1 argument(path to file) in which you want to detect faces\n";
        return 0;
    }

    constexpr auto kApi  = std::string_view{ "https://vision.googleapis.com/v1/images:annotate?key=API_KEY" };
    const auto kPayload  = GetJsonPayloadForFaceDetectionApi(ReadFileToBase64(argv[1]));
    auto network_manager = QNetworkAccessManager{};
    auto request         = QNetworkRequest{ QUrl{ kApi.data() } };
    request.setHeader(QNetworkRequest::ContentTypeHeader, "Content-Type: application/json; charset=utf-8");

    auto *network_reply = network_manager.post(request, kPayload.toUtf8());
    auto response = QByteArray{};
    QObject::connect(network_reply, &QNetworkReply::readyRead, [network_reply, &response]()
    {
        response += network_reply->readAll();
    });
    QObject::connect(network_reply, &QNetworkReply::finished, [network_reply, &response]()
    {
        HandleResponse(response);
        network_reply->deleteLater();
        qApp->quit();
    });
    return a.exec();
}
