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
 * This program acts as the client. Which sends the numbers to server and wait for server to respond.
 * Server response is printed on client.
 * 
 * Driver code :
 * - Initiates the connection with server on port 1234.
 * - Ask the user to input a number.
 * - Test if the number is between 1 and 99.
 * - Send the number to server and wait for its response.
 * - Print the server response on console.
 * - Loop again
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <QCoreApplication>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

#include <iostream>
#include <tuple>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::tuple;

bool IsValueInRange(const int &value, const int &min, const int &max)
{
    return value >= min && value <= max;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    auto tcp_socket = QTcpSocket{};
    tcp_socket.connectToHost(QHostAddress::LocalHost, 1234);
    if (false == tcp_socket.waitForConnected(-1)) { qDebug() << tcp_socket.errorString(); }
    else
    {
        cout << "Socket connected" << endl;
        for (auto [user_input, buffer] = tuple{ 0, std::string{} }; true;)
        {
            cout << "Enter number between 1 and 99: ";
            cin >> user_input;
            if (false == IsValueInRange(user_input, 1, 99)) { break; }
            else
            {
                buffer = std::to_string(user_input);
                if (-1 == tcp_socket.write(buffer.c_str())) { cout << "Unable to write bytes" << endl; }
                else
                {
                    if (false == tcp_socket.waitForBytesWritten(-1)) { cout << "No bytes sent" << endl; }
                    else
                    {
                        if (false == tcp_socket.waitForReadyRead(-1)) { cout << "No bytes read" << endl; }
                        else
                        {
                            auto data_from_server = tcp_socket.readAll();
                            cout << "Server response: " << data_from_server.toStdString() << endl;
                        }
                    }
                }
            }
        }
    }

    cout << "Client Exiting";
    return 0;
}
