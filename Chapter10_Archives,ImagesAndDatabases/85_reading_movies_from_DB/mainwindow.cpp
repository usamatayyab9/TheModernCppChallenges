#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    UpdateMovieList();
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_back_button_released()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::UpdateMovieList()
{
    ReadMoviesFromDB();
    ui->movies_list_widget->clear();
    for (const auto &kMovie : movies_)
    {
        ui->movies_list_widget->addItem(QString{"%1 (%2)"}.arg(kMovie.title).arg(kMovie.release_year));
    }
}

void MainWindow::ReadMoviesFromDB()
{
    movies_.clear();
    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(kDatabaseFile.data());
    if (!db.open()) { qDebug() << "DB opening error : " << db.lastError().text(); }
    else
    {
        qDebug() << "DB Opened successfully";
        auto query = QSqlQuery{};
        query.prepare("SELECT movieid, title,year,length FROM movies");
        if (query.exec())
        {
            while (query.next())
            {
                const auto kID      = query.value("movieid").toULongLong();
                const auto kTitle   = query.value("title").toString();
                const auto kYear    = query.value("year").toInt();
                const auto kRunTime = query.value("length").toInt();
                qDebug() << "Movie ID      : " << kID;
                qDebug() << "Movid TITLE   : " << kTitle;
                qDebug() << "Movid YEAR    : " << kYear;
                qDebug() << "Movid DURATION: " << kRunTime;
                qDebug() << "";
                movies_.push_back(Movie<QString>{ kID, kTitle, kYear, std::chrono::minutes{ kRunTime } });
            }
        }
    }
    db.removeDatabase(kDatabaseFile.data());
    db.close();
}

void MainWindow::SetupDetailsPage()
{
    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(kDatabaseFile.data());
    if (!db.open()) { qDebug() << "DB opening error : " << db.lastError().text(); }
    else
    {
        const auto kCurrentIdx = static_cast<size_t>(ui->movies_list_widget->currentRow());
        qDebug() << QString::number(movies_[kCurrentIdx].id);
        qDebug() << movies_[kCurrentIdx].title;
        qDebug() << movies_[kCurrentIdx].release_year;
        qDebug() << GetDurationText(movies_[kCurrentIdx].length_minutes);

        ui->id_label->setText(QString::number(movies_[kCurrentIdx].id));
        ui->title_label_2->setText(movies_[kCurrentIdx].title);
        ui->year_label_2->setNum(movies_[kCurrentIdx].release_year);
        ui->runtime_label_2->setText(GetDurationText(movies_[kCurrentIdx].length_minutes));
        PopulateDirectorsList();
        PopulateWritersList();
        PopulateCastingTable();
    }
    db.removeDatabase(kDatabaseFile.data());
    db.close();
}

void MainWindow::PopulateDirectorsList()
{
    const auto kCurrentIdx = static_cast<size_t>(ui->movies_list_widget->currentRow()); //static_cast to avoid warnings
    auto query             = QSqlQuery{};
    query.prepare("SELECT name FROM persons,directors where persons.personid=directors.personid AND directors.movieid=?");
    query.addBindValue(QVariant::fromValue(movies_[kCurrentIdx].id));
    movies_[kCurrentIdx].directors.clear();
    if (query.exec())
    {
        while (query.next())
        {
            movies_[kCurrentIdx].directors.push_back(query.value("name").toString());
        }
    }
    ui->directors_list->clear();
    for (const auto &kDirector : movies_[kCurrentIdx].directors)
    {
        ui->directors_list->addItem(kDirector);
    }
}

void MainWindow::PopulateWritersList()
{
    const auto kCurrentIdx = static_cast<size_t>(ui->movies_list_widget->currentRow()); //static_cast to avoid warnings
    auto query             = QSqlQuery{};
    query.prepare("SELECT name FROM persons,writers where persons.personid=writers.personid AND writers.movieid=?");
    query.addBindValue(QVariant::fromValue(movies_[kCurrentIdx].id));
    movies_[kCurrentIdx].writers.clear();
    if (query.exec())
    {
        while (query.next())
        {
            movies_[kCurrentIdx].writers.push_back(query.value("name").toString());
        }
    }
    ui->writers_list->clear();
    for (const auto &kWriter : movies_[kCurrentIdx].writers)
    {
        ui->writers_list->addItem(kWriter);
    }
}

void MainWindow::PopulateCastingTable()
{
    const auto kCurrentIdx = static_cast<size_t>(ui->movies_list_widget->currentRow()); //static_cast to avoid warnings
    auto query             = QSqlQuery{};
    query.prepare("SELECT persons.name, casting.role FROM persons,casting where persons.personid=casting.personid AND casting.movieid=?");
    query.addBindValue(QVariant::fromValue(movies_[kCurrentIdx].id));
    movies_[kCurrentIdx].star_cast.clear();
    if (query.exec())
    {
        while (query.next())
        {
            movies_[kCurrentIdx].star_cast.push_back({ query.value("name").toString(), query.value("role").toString() });
        }
    }
    ui->casting_list->clear();
    for (const auto &[kActor, kRole] : movies_[kCurrentIdx].star_cast)
    {
        ui->casting_list->addItem(QString{ "%1\tAS\t%2" }.arg(kActor).arg(kRole));
    }
}

QString MainWindow::GetDurationText(const std::chrono::minutes &duration)
{
    auto hours            = duration / 1h;
    auto remainin_minutes = duration % 1h;
    auto str = QString{};
    if (hours > 0)
    {
        str.append(QString::number(hours)).append("h ");
    }
    str.append(QString::number(remainin_minutes.count())).append("m");
    return str;
}

void MainWindow::on_update_button_released()
{
    UpdateMovieList();
}

void MainWindow::on_movies_list_widget_itemClicked(QListWidgetItem *item)
{
    qDebug() << ui->movies_list_widget->currentRow();
    SetupDetailsPage();
    ui->stackedWidget->setCurrentIndex(1);
}
