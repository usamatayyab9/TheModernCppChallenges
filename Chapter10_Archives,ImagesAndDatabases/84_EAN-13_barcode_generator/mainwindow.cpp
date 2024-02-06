#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief This slot is called whenever user clicks the generate button,
 * 
 * 
 */
void MainWindow::on_pushButton_released()
{
    try
    {
        auto ean_13_number = EAN_13Number(ui->textEdit->toPlainText().toStdString());
        auto image         = barcode_generator_(ean_13_number);
        if (image.isNull())
        {
            ui->label->setText("Unable to generate barcode");
        }
        else
        {
            ui->label->setText("");
            ui->label->setPixmap(QPixmap::fromImage(image));
        }
    } catch (const std::exception &e)
    {
        ui->label->setText("Invalid EAN-13 provided");
        qDebug() << e.what();
    }
}
