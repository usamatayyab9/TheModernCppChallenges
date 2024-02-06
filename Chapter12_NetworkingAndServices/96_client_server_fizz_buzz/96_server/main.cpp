/**
 * @file main.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compiled with Qt 5.11.3 with gcc 9.4.0
 * 
 * This file is solution to "Problem 96. Client-server Fizz-Buzz"
 * mentioned in "Chapter 12 : Networking and Services" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * This program acts as the server. Which listens to the server on port 1234. Server runs indefinitely.
 * Server signal is connected to lambda which is invoked whenever there is a new connection. And once a 
 * bytes are available on the server then lambda connected to readReady signal is invoked. Server runs
 * asynchronously.
 * 
 * Driver code :
 * - Initiates the connection on port 1234.
 * - Asynchronously waits for a new connection.
 * - Once the data is available on new connection.
 * - Convert the bytes to integer and get FizzBuzz string.
 * - Writes the string back to client.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QObject>

#include <string>

std::string NumToFizzBuzz(const int &num)
{
    auto str = std::string{};
    if (0 == (num % 3)) { str += "Fizz"; }
    if (0 == (num % 5)) { str += "Buzz"; }
    if (str.empty())    { str = std::to_string(num); }
    return str;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    auto tcp_server = QTcpServer{};
    if (tcp_server.listen(QHostAddress::LocalHost, 1234))
    {
        qDebug() << "Server is listening";
        QObject::connect(&tcp_server, &QTcpServer::newConnection, [&tcp_server]()
        {
            qDebug() << "New connection received";
            auto client_tcp_socket = tcp_server.nextPendingConnection();
            QObject::connect(client_tcp_socket, &QTcpSocket::readyRead, [client_tcp_socket]()
            {
                const auto kDataFromClient = client_tcp_socket->readAll();
                const auto kResponse       = NumToFizzBuzz(kDataFromClient.toInt());
                client_tcp_socket->write(kResponse.c_str());
                qDebug() << "Client request: " << kDataFromClient;
                qDebug() << "Bytes written: " << kResponse.c_str();
            });

        });
    }

    return a.exec();
}
