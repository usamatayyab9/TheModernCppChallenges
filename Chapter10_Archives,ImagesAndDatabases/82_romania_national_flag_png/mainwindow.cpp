#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Slot called when user clicks generate button
 * 
 */
void MainWindow::on_generate_button_released()
{
    auto flag_image = flag_png_generator_(ui->width_spinbox->value(), ui->height_spinbox->value(), ui->file_path_textedit->toPlainText());
    if (false == flag_image.isNull())
    {
        ui->flag_label->setPixmap(QPixmap::fromImage(flag_image));
    }
    else
    {
        ui->flag_label->setText("Unable to generate PNG");
    }
}
