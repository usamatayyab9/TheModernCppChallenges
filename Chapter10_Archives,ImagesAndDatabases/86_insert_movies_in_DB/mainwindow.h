#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QListWidgetItem>
#include <QIntValidator>
#include <QTableWidget>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>

#include <limits>
#include <iostream>
#include <string_view>

#include "movie.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

inline constexpr auto kDatabaseFile = std::string_view{ "../85_86_movies.db" };

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    template<class StringType> QDebug Print(const Movie<StringType> &movie);
    template<class T, class RecordQueryCreator> void InsertRecords(const size_t movie_id, const vector<T> &vec, RecordQueryCreator query_creator);

private slots:
    void AddItemToList(QListWidget *list_widget);
    void DeleteCurrentItemFromList(QListWidget *list_widget);
    bool ListWidgetCheck(QListWidget *);
    bool CastTableWidgetCheck();
    vector<QString> GetNonEmptyElementsFromList(QListWidget *list_widget);
    vector<ActorCast<QString>> GetNonEmptyElementsFromTable(QTableWidget *list_widget);
    size_t GetPersonID(const QString &str);
    void InsertPerson(const QString &str);
    void InsertDirectors(const size_t &movie_id, const std::vector<QString> &directors);
    void InsertWriters(const size_t &movie_id, const std::vector<QString> &writers);
    void InsertCast(const size_t &movie_id, const std::vector<ActorCast<QString>> &actor_cast);
    void InsertMovieIntoDb(const Movie<QString> &movie);
    void on_add_cast_pushButton_released();
    void on_delete_cast_pushButton_released();
    void on_insert_movie_pushButton_released();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
