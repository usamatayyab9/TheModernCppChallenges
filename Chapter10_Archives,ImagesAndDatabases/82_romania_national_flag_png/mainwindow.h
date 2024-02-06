#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "romaniaflagpnggenerator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_generate_button_released();

private:
    Ui::MainWindow *ui;
    RomaniaFlagPNGGenerator flag_png_generator_;/*! A functor object for generating romania flag image.*/
};
#endif // MAINWINDOW_H
