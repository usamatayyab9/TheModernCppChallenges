#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->add_director_pushButton, &QPushButton::released, std::bind(&MainWindow::AddItemToList, this, ui->directors_listWidget));
    connect(ui->delete_director_pushButton, &QPushButton::released, std::bind(&MainWindow::DeleteCurrentItemFromList, this, ui->directors_listWidget));
    connect(ui->add_writer_pushButton, &QPushButton::released, std::bind(&MainWindow::AddItemToList, this, ui->writers_listWidget));
    connect(ui->delete_writer_pushButton, &QPushButton::released, std::bind(&MainWindow::DeleteCurrentItemFromList, this, ui->writers_listWidget));
    ui->year_lineEdit->setValidator(new QIntValidator(1200, std::numeric_limits<int>::max(), ui->year_lineEdit));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddItemToList(QListWidget *list_widget)
{
    auto row  = list_widget->count();
    auto item = new QListWidgetItem{};
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    list_widget->insertItem(row, item);
    list_widget->setCurrentItem(item);
    list_widget->editItem(item);
}

void MainWindow::DeleteCurrentItemFromList(QListWidget *list_widget)
{
    if (auto current_item = list_widget->takeItem(list_widget->currentRow()); current_item)
    {
        qDebug() << current_item->text();
        delete current_item;
    }
}

/**
 * @brief MainWindow::ListWidgetCheck
 * @param list_widget
 * @return
 * true if list contains any non-empty element
 * false, otherwise
 */
bool MainWindow::ListWidgetCheck(QListWidget *list_widget)
{
    auto is_valid = false;
    for (auto idx = 0; idx < list_widget->count() ;++idx)
    {
        if (list_widget->item(idx)->text().size() > 0)
        {
            is_valid = true;
            break;
        }
    }
    return is_valid;
}

bool MainWindow::CastTableWidgetCheck()
{
    auto is_valid = false;
    for (auto idx = 0; idx < ui->cast_tableWidget->rowCount() ;++idx)
    {
        auto actor_item = ui->cast_tableWidget->item(idx, 0);
        auto role_item  = ui->cast_tableWidget->item(idx, 1);
        if (actor_item && role_item && actor_item->text() > 0 && role_item->text() > 0)
        {
            is_valid = true;
            break;
        }
    }
    return is_valid;
}

vector<QString> MainWindow::GetNonEmptyElementsFromList(QListWidget *list_widget)
{
    auto vec = vector<QString>{};
    for (auto idx = 0; idx < list_widget->count() ;++idx)
    {
        if (const auto kText = list_widget->item(idx)->text(); kText.size() > 0)
        {
            vec.push_back(kText);
        }
    }
    return vec;
}

vector<ActorCast<QString>> MainWindow::GetNonEmptyElementsFromTable(QTableWidget *table_widget)
{
    auto tables_cells = vector<ActorCast<QString>>{};
    for (auto row_idx = 0; row_idx < table_widget->rowCount() ;++row_idx)
    {
        auto actor_item = ui->cast_tableWidget->item(row_idx, 0);
        auto role_item  = ui->cast_tableWidget->item(row_idx, 1);
        if (actor_item && role_item && actor_item->text() > 0 && role_item->text() > 0)
        {
            tables_cells.push_back({ actor_item->text(), role_item->text() });
        }
    }
    return tables_cells;
}

size_t MainWindow::GetPersonID(const QString &str)
{
    auto query = QSqlQuery{};
    query.prepare("SELECT personid FROM persons WHERE name=?");
    query.addBindValue(str);
    auto person_id = 0;
    if (query.exec() && query.first())
    {
        person_id = query.value(0).toInt();
        qDebug() << "Person ID: " << person_id;
    }
    return person_id;
}

void MainWindow::InsertPerson(const QString &str)
{
    auto query = QSqlQuery{};
    query.prepare("INSERT INTO persons(name) VALUES (?)");
    query.addBindValue(str);
    if (query.exec())
    {
        qDebug() << "Person inserted successfully";
    }
}

void MainWindow::InsertDirectors(const size_t &movie_id, const std::vector<QString> &directors)
{
    InsertRecords(movie_id, directors, [](QSqlQuery &query, const auto &kMovieId, const auto &kPersonId){
        query.prepare("INSERT INTO directors(movieid, personid) VALUES(:movieid, :personid)");
        query.bindValue(":movieid", QVariant::fromValue(kMovieId));
        query.bindValue(":personid", QVariant::fromValue(kPersonId));
    });
}

void MainWindow::InsertWriters(const size_t &movie_id, const std::vector<QString> &writers)
{
    InsertRecords(movie_id, writers, [](QSqlQuery &query, const size_t &kMovieId, const size_t &kPersonId){
        query.prepare("INSERT INTO writers(movieid, personid) VALUES(:movieid, :personid)");
        query.bindValue(":movieid", QVariant::fromValue(kMovieId));
        query.bindValue(":personid", QVariant::fromValue(kPersonId));
    });
}

void MainWindow::InsertCast(const size_t &movie_id, const std::vector<ActorCast<QString> > &actor_cast)
{
    auto query = QSqlQuery{};
    for (const auto &[kRealName, kMovieName] : actor_cast)
    {
        auto person_id = GetPersonID(kRealName);
        if (0 == person_id)
        {
            InsertPerson(kRealName);
            person_id = GetPersonID(kRealName);
        }
        query.prepare("INSERT INTO casting(movieid, personid, role) VALUES(:movieid, :personid, :role)");
        query.bindValue(":movieid", QVariant::fromValue(movie_id));
        query.bindValue(":personid", QVariant::fromValue(person_id));
        query.bindValue(":role", kMovieName);
        if (query.exec())
        {
            qDebug() << "Cast " << kRealName + " as " + kMovieName  << "inserted successfully: ";
        }
    }
}

void MainWindow::InsertMovieIntoDb(const Movie<QString> &movie)
{
    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(kDatabaseFile.data());
    if (!db.open()) { qDebug() << "DB opening error : " << db.lastError().text(); }
    else
    {
        qDebug() << "DB Opened successfully";
        auto query = QSqlQuery{};
        query.prepare("INSERT INTO movies(title, year, length) VALUES (:title, :year, :length)");
        query.bindValue(":title", movie.title);
        query.bindValue(":year", movie.release_year);
        query.bindValue(":length", QVariant::fromValue(movie.length_minutes.count()));
        if (!query.exec()) { qDebug() << "Query Execution failed: " << query.lastError().text(); }
        else
        {
            auto movie_id = query.lastInsertId().toULongLong();
            qDebug() << "Movie inserted successfull";
            qDebug() << "Movie ID: " << movie_id;
            InsertDirectors(movie_id, movie.directors);
            InsertWriters(movie_id, movie.writers);
            InsertCast(movie_id, movie.star_cast);
        }
    }
    db.removeDatabase(kDatabaseFile.data());
    db.close();
}

void MainWindow::on_add_cast_pushButton_released()
{
    auto row  = ui->cast_tableWidget->rowCount();
    ui->cast_tableWidget->insertRow(row);
}

void MainWindow::on_delete_cast_pushButton_released()
{
    qDebug() << ui->cast_tableWidget->currentRow();
    ui->cast_tableWidget->removeRow(ui->cast_tableWidget->currentRow());
}

void MainWindow::on_insert_movie_pushButton_released()
{
    const auto kTitle    = ui->title_lineEdit->text();
    const auto kYear     = ui->year_lineEdit->text();
    const auto kDuration = ui->lenght_hours_spinBox->value() * 60 + ui->lenght_minutes_spinBox->value();
    if (kTitle.size() > 0
        && kYear.size() > 0
        && kDuration > 0
        && ListWidgetCheck(ui->directors_listWidget)
        && ListWidgetCheck(ui->writers_listWidget)
        && CastTableWidgetCheck())
    {
        qDebug() << "Valid Data";
        auto movie           = Movie<QString>{};
        movie.title          = kTitle;
        movie.release_year   = kYear.toInt();
        movie.length_minutes = minutes{ kDuration };
        movie.directors      = GetNonEmptyElementsFromList(ui->directors_listWidget);
        movie.writers        = GetNonEmptyElementsFromList(ui->writers_listWidget);
        movie.star_cast     = GetNonEmptyElementsFromTable(ui->cast_tableWidget);
        Print(movie);
        InsertMovieIntoDb(movie);
    }
    else
    {
        qDebug() << "Incomplete fields";
    }
}

template<class StringType>
QDebug MainWindow::Print(const Movie<StringType> &movie)
{
    auto PrintVector = [](const auto &vec, auto &out){
        for (const auto &elem : vec)
        {
            out << "\t" << elem << "\n";
        }
    };
    auto out = qDebug();
    out << "id        : " << movie.id << '\n';
    out << "title     : " << movie.title << '\n';
    out << "year      : " << movie.release_year << '\n';
    out << "minutes   : " << movie.length_minutes.count() << '\n';
    out << "directors :\n"; PrintVector(movie.directors, out);
    out << "writers   :\n"; PrintVector(movie.writers, out);
    out << "cast      :\n";
    for (const auto &[kRealName, kReelName] : movie.star_cast)
    {
        out << "\t" << kRealName << " as " << kReelName << '\n';
    }
    return out;
}

template<class T, class RecordQueryCreator>
void MainWindow::InsertRecords(const size_t movie_id, const vector<T> &vec, RecordQueryCreator query_creator)
{
    auto query = QSqlQuery{};
    for (const auto &kElement : vec)
    {
        auto person_id = GetPersonID(kElement);
        if (0 == person_id)
        {
            InsertPerson(kElement);
            person_id = GetPersonID(kElement);
        }
        qDebug() << "Person ID: " << person_id;
        query_creator(query, movie_id, person_id);
        if (query.exec())
        {
            qDebug() << "Record " << kElement <<  " inserted successfully: ";
        }
    }
}
