/**
 * @file main.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compiled with Qt 5.11.3 with gcc 9.4.0
 * 
 * This file is solution to "Problem 82. Creating a PNG that represents a national flag"
 *  mentioned in "Chapter 10 : Archives, Images, and Databases" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * 
 * Solution:
 * - A class `RomaniaFlagPNGGenerator` for generating flag image. Provides an overload for function
 * call operator and returns the image. See class comments for more details.
 * 
 * - A class `MainWindow` for user interface. The UI consists requires the user to specify
 * width and height of flag image by default it is already set to 100. After that user can 
 * click Generate button and the image will be displayed.
 * 
 * @note IMPORTANT FOR LAUNCHING THIS PROGRAM
 * Driver code:
 * This program can be launched using two methods. One is UI mode and second is CLI mode.
 * - UI mode:
 *      To launch the UI simply run the program without any additional argument. Once the UI
 *      appears user can enter height and width of image and file name and click the generate button.
 *      After that program genrates the image, saves it with the user provided file name and displays
 *      the image on UI too.
 * - CLI mode:
 *      To launch the CLI mode program should be launched with 3 additional arguments i.e. "width",
 *      "height" and "output". Output is the name the image file that. Qt `QCommandLineParser`
 *      class  is used for parsing command line arguments. On completion program prints success or Error.
 *      If the program is launched with wrong arguments then prints help text and exits. 
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

#include "romaniaflagpnggenerator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << argc;
    if (1 == argc) //If no arguments, launch GUI
    {
        MainWindow w;
        w.show();
        return a.exec();
    }
    else
    {
        auto command_line_args_parser = QCommandLineParser{};
        command_line_args_parser.setApplicationDescription("This application generates flag of Romania and store it as png file.");
        command_line_args_parser.addHelpOption();
        command_line_args_parser.addOptions({
            { "width", "width of PNG file", "value" },
            { "height", "height of PNG file", "value" },
            { "output", "Path where PNG file will be saved", "file" }
        });
        command_line_args_parser.process(a);
        if (command_line_args_parser.isSet("width")
                && command_line_args_parser.isSet("height")
                && command_line_args_parser.isSet("output"))
        {
            auto width  = command_line_args_parser.value("width").toInt();
            auto height = command_line_args_parser.value("height").toInt();
            auto file   = command_line_args_parser.value("output");
            qDebug() << "width: " << width << ", height: " << height << ", output: " << file;
            auto flag_generator = RomaniaFlagPNGGenerator{};
            auto image          = flag_generator(width, height, file);
            qDebug() << (image.isNull() ? "Error" : "Success");
        }
        else
        {
            command_line_args_parser.showHelp();
        }

        return 0;
    }
}
