#ifndef ADDMEDIADIALOG_H
#define ADDMEDIADIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QDebug>
#include <QString>
#include <QMessageBox>

#include <tuple>

namespace Ui {
class AddMediaDialog;
}

class AddMediaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMediaDialog(QWidget *parent = nullptr);
    ~AddMediaDialog();
    bool IsAccepted() const { return is_accepted_; }
    std::tuple<QString, QString, QString> GetMediaFields() const
    {
        return { file_path_, name_, description_ };
    }

private slots:
    void on_select_media_file_button_released();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::AddMediaDialog *ui;
    bool    is_accepted_{ false };
    QString file_path_{};
    QString name_{};
    QString description_{};
};

#endif // ADDMEDIADIALOG_H
