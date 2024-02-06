#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QByteArray>
#include <QListWidgetItem>
#include <QPixmap>

#include <string_view>
#include <vector>

#include "addmediadialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

inline constexpr auto kDatabaseFile = std::string_view{ "../87_movies_media.db" };

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_find_button_released();
    void on_add_media_button_released();
    void on_list_meadia_id_button_released();

    void on_delete_media_button_released();

private:
    void UpdateMediaList(const int &movidid);
    bool AddMediaToDB(const int &movie_id, const QString &file_path, const QString &name, const QString &desc);
    bool DeleteMediaByID(const int &media_id);
    Ui::MainWindow *ui;
    QSqlDatabase     db_;
    QSqlTableModel*  model_;
    std::vector<int> media_ids_;
    int              current_movie_id_{ -1 };

};
#endif // MAINWINDOW_H
