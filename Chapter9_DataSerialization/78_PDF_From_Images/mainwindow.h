#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QPixmap>

#include <tuple>

#include "imagepdfwriter.h"

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
    void on_browse_button_released();
    void on_generate_pdf_button_released();
    void GeneratePDF();
    QString GetOutputFilename();
private:
    Ui::MainWindow*  ui;
    QFileSystemModel file_model_;
};
#endif // MAINWINDOW_H
