#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    file_model_.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    file_model_.setNameFilters({ "*.jpg", "*.jpeg", "*.png" });
    file_model_.setNameFilterDisables(false);
    file_model_.setRootPath("");
    ui->files_list_view->setModel(&file_model_);
    ui->message_label->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_browse_button_released()
{
    qDebug() << "Browse button pressed";
    auto file_dialog = QFileDialog(this, "Select directory for images");
    file_dialog.setOptions(QFileDialog::ShowDirsOnly);
    file_dialog.setFileMode(QFileDialog::DirectoryOnly);
    file_dialog.exec();

    ui->path_label->setText(file_dialog.selectedFiles().first());
    ui->files_list_view->setRootIndex(file_model_.setRootPath(file_dialog.selectedFiles().first()));
}

void MainWindow::on_generate_pdf_button_released()
{
    qDebug() << "Generating PDF...";
    ui->path_label->setEnabled(false);
    ui->browse_button->setEnabled(false);
    ui->files_list_view->setEnabled(false);
    ui->message_label->setText("Generating PDF...Please wait");
    ui->message_label->show();
    qApp->processEvents();
    emit GeneratePDF();
}

/**
 * @brief Function for generating pdf file of images.
 * Images are fetched from user selected directory. All
 * the images found in the directory will be written to
 * pdf file.
 * 
 */
void MainWindow::GeneratePDF()
{
    qDebug() << file_model_.rowCount(file_model_.index(file_model_.rootPath()));
    auto root_index      = file_model_.index(file_model_.rootPath());
    const auto kFilename = GetOutputFilename();
    auto writer          = QPdfWriter{ kFilename };

    writer.setPageSize(QPageSize(QPageSize::A4));
    auto painter = QPainter(&writer);
    /*! Traverses all the objects of file model. */
    for (auto [idx, rowCount, vertical_height] = std::tuple{ 0 , file_model_.rowCount(root_index), 0 };
         idx < rowCount;
         ++idx)
    {
        if (auto index  = file_model_.index(idx, 0, root_index); false == file_model_.isDir(index))
        {
            auto filepath = file_model_.filePath(index);
            auto pixmap   = QPixmap(filepath);
            qDebug() << "pixmap status: " << pixmap.isNull();
            qDebug() << "Writing: " << filepath;
            /*Insert a new once we current page height reaches a threshold*/
            if (vertical_height + pixmap.height() + 500 > writer.height())
            {
                writer.newPage();
                vertical_height = 0;
            }
            painter.drawPixmap(0, vertical_height, pixmap);
            vertical_height += pixmap.height() + 500;/*Leave space of 500 units between two images.*/
        }
    }
    ui->message_label->setText("PDF file generated");
    ui->path_label->setEnabled(true);
    ui->browse_button->setEnabled(true);
    ui->files_list_view->setEnabled(true);
}

QString MainWindow::GetOutputFilename()
{
    return ui->output_filename_label->toPlainText().isEmpty() ? "output.pdf"
                                                              : ui->output_filename_label->toPlainText();
}
