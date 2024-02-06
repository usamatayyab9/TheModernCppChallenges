#include "addmediadialog.h"
#include "ui_addmediadialog.h"

AddMediaDialog::AddMediaDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMediaDialog)
{
    ui->setupUi(this);
}

AddMediaDialog::~AddMediaDialog()
{
    delete ui;
}

void AddMediaDialog::on_select_media_file_button_released()
{
    auto file_name = QFileDialog::getOpenFileName(this, tr("Select Image"), "./", tr("Image Files (*.png *.jpg *.svg)"));
    qDebug() << file_name;
    ui->media_file_label->setText(file_name);
}

void AddMediaDialog::on_buttonBox_accepted()
{
    if (ui->media_file_label->text().isEmpty())
    {
        auto alert_box = QMessageBox(QMessageBox::Icon::Warning, "Alert", "Please select media file id to add");
        alert_box.exec();
    }
    else
    {
        is_accepted_ = true;
        file_path_   = ui->media_file_label->text();
        name_        = ui->media_name_text_edit->toPlainText();
        description_ = ui->media_desc_text_edit->toPlainText();
    }
}

void AddMediaDialog::on_buttonBox_rejected()
{
    is_accepted_ = false;
    file_path_.clear();
    name_.clear();
    description_.clear();
}
