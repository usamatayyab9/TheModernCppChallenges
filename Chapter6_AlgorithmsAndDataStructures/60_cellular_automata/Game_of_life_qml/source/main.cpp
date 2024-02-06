/**
 * @file main.cpp
 * @author Usama Tayyab (usamtayyab9@gmail.com)
 * @brief 
 * Compilation is done with Qt 5.11.3 with gcc 9.4.0
 * 
 *  This file is solution to "Problem 60. The Game of Life"
 *  mentioned in "Chapter 6: Algorithms and Data Structures" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * The version of game of life is implemented using Qt C++.
 * Following class are used to in implementation:
 * 
 * - Class `Grid` declared in grid.h and defined in grid.cpp.
 *      Provides a 2-dimensional grid on which game of life operates.
 *      This class provides all neccessary functionality for accessing
 *      grid dimension i.e. count of rows and columns, constructing the
 *      grid with sepcified number of row and columns, accessing/updating
 *      any cell of grid via index operator overload.
 * 
 * - Class `CellularAutomata` which is an abstract class and acts as
 *      one of parent class of `GameOfLife`. The class `CellularAutomata`
 *      provides a protected grid and provides neccessary functions
 *      for initalizing internal grid with certain dimensions, setting 
 *      all cells of grid, clearing all cells of grid, random setting
 *      state of each cell. This class has pure virtual function `recalculate()`,
 *      it is responsibility of inheriting class (in our case `GameOfLife`)
 *      to implement this function and update grid accordingly.
 *   
 * - Class `GameOfLife` which inherits two classes QObject and `CellularAutomata`.
 *      It is inherited with QObject so that it can be used as Qt class
 *      and can be registered with qml. Most of the functions in this class
 *      are marked with Q_INVOKABLE so that they can be called from qml file.
 *      This class defined the virtual function `recalculate()` which
 *      implements the game of life logic. This class object is instantiated
 *      in main.qml file. This class provides all functionality for fetching
 *      state of each cell i.e. alive or dead, setting/clearing the entire
 *      grid and randmizing state of each. All of these functions are called
 *      from qml.
 * 
 * - main.qml
 *      This file implements all of the UI. All functionality related
 *      to UI i.e. displaying the grid, buttons for setting/clearing
 *      randomizing the grid, periodically updating the grid etc 
 * 
 * Driver code:
 * - Intializes Qt specific objects.
 * - Registers `GameOfLife` class in qml
 * - Execute the application
 * 
 * For more details view file and class comments.
 * 
 * 
 * @copyright Copyright (c) 2024
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "gameoflife.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    qmlRegisterType<GameOfLife>("cellularAutomata.gameOfLife", 1, 0, "GameOfLife");
    engine.load(url);
    qDebug()<<"Running application";
    return app.exec();
}
