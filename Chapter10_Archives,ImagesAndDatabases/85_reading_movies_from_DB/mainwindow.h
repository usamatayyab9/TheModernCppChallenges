#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QListWidgetItem>

#include <string_view>
#include <chrono>

#include "movie.h"

using namespace std::chrono_literals;
using MoviesList = std::vector<Movie<QString>>;

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

private slots:
    void on_back_button_released();
    void on_update_button_released();
    void on_movies_list_widget_itemClicked(QListWidgetItem *item);

private:
    void UpdateMovieList();
    void ReadMoviesFromDB();
    void SetupDetailsPage();
    void PopulateDirectorsList();
    void PopulateWritersList();
    void PopulateCastingTable();
    QString GetDurationText(const std::chrono::minutes &duration);

    Ui::MainWindow *ui;
    /*
     * members directors,writers, cast will only be populated when user wants to view the details
     * otherwise they will be empty and only id, title,release_year and duration will be initialized
     * for displaying basic movie info on 1st page. When user clicks on a movie he will taken to
     * 2nd page where he can view movie details.
     */
    MoviesList movies_{};
};
#endif // MAINWINDOW_H
