#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(kDatabaseFile.data());
    if (!db_.open()) { qDebug() << "DB opening error : " << db_.lastError().text(); }
    else
    {
        qDebug() << "DB Opened successfully";
        model_ = new QSqlTableModel(this, db_);
        model_->setTable("movies");
        model_->setHeaderData(0, Qt::Horizontal, tr("ID"));
        model_->setHeaderData(1, Qt::Horizontal, tr("Title"));
        model_->setHeaderData(2, Qt::Horizontal, tr("Year"));
        model_->setHeaderData(3, Qt::Horizontal, tr("Length"));

        ui->movie_info_table->setModel(model_);
        ui->movie_info_table->show();
    }
    qDebug() << "media table " << db_.tables();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_find_button_released()
{
    if (ui->find_movie_title_text_edit->toPlainText().isEmpty())
    {
        auto alert_box = QMessageBox(QMessageBox::Icon::Warning, "Alert", "Please input a movie title to find");
        alert_box.exec();
    }
    else
    {
        model_->setFilter("title= '" + ui->find_movie_title_text_edit->toPlainText() + "'");
        qDebug() << model_->select();
        qDebug() << model_->lastError().text();
    }
}

void MainWindow::on_add_media_button_released()
{
    if (ui->list_media_id_text_edit->toPlainText().isEmpty())
    {
        auto alert_box = QMessageBox(QMessageBox::Icon::Warning, "Alert", "Please input a movie id to add media");
        alert_box.exec();
    }
    else
    {
        auto add_media_dialog = AddMediaDialog{};
        add_media_dialog.exec();
        if (add_media_dialog.IsAccepted())
        {
            const auto kMovieId = ui->list_media_id_text_edit->toPlainText().toInt();
            const auto [kFilePath, kName, kDescription] = add_media_dialog.GetMediaFields();
            current_movie_id_ = kMovieId;
            if (const auto kIsSuccess = AddMediaToDB(kMovieId, kFilePath, kName, kDescription); kIsSuccess)
            {
                UpdateMediaList(kMovieId);
            }
        }
    }
}

void MainWindow::on_list_meadia_id_button_released()
{
    if (ui->list_media_id_text_edit->toPlainText().isEmpty())
    {
        auto alert_box = QMessageBox(QMessageBox::Icon::Warning, "Alert", "Please input a movie id to display media");
        alert_box.exec();
    }
    else
    {
        ui->media_operation_status_label->setText("Search media operation successfull");
        current_movie_id_ = ui->list_media_id_text_edit->toPlainText().toInt();
        UpdateMediaList(ui->list_media_id_text_edit->toPlainText().toInt());
    }
}

void MainWindow::UpdateMediaList(const int &movieid)
{
    auto query = QSqlQuery{};
    query.prepare("SELECT mediaid,name, description,content FROM media WHERE movieid=?");
    query.addBindValue(movieid);
    if (query.exec())
    {
        ui->media_table_widget->clear();
        media_ids_.clear();
        while (query.next())
        {
            const auto kName = query.value("name").toString();
            const auto kDesc = query.value("description").toString();
            const auto kContent = query.value("content").toByteArray();
            media_ids_.push_back(query.value("mediaid").toInt());
            qDebug() << kName << ", " << kDesc << ", " << kContent.data();

            auto pix_map = QPixmap{};
            pix_map.loadFromData(kContent);
            auto item   = new QListWidgetItem{};
            auto label  = new QLabel(ui->media_table_widget);

            item->setSizeHint(pix_map.size());
            label->setPixmap(pix_map);

            ui->media_table_widget->addItem(item);
            ui->media_table_widget->setItemWidget(item, label);
        }
    }
    else
    {
        qDebug() << query.lastError().text();
        ui->media_operation_status_label->setText("Search media operation failed");
    }
}

bool MainWindow::AddMediaToDB(const int &movie_id, const QString &file_path, const QString &name, const QString &desc)
{
    qDebug() << file_path << ", " << name << ", " << desc;
    auto file = QFile{ file_path };
    qDebug() << "File status" << file.open(QIODevice::ReadOnly);
    auto media_data = file.readAll();
    qDebug() << "Data:" << media_data.data();
    auto query = QSqlQuery{};
    query.prepare("INSERT INTO media(movieid, name, description, content) VALUES (:movieid, :name, :desc, :content)");
    query.bindValue(":movieid", movie_id);
    query.bindValue(":name", name);
    query.bindValue(":desc", desc);
    query.bindValue(":content", media_data);
    auto flag = false;
    if (!query.exec())
    {
        qDebug() << query.lastError().text();
        ui->media_operation_status_label->setText("Add media operation failed");
        flag = false;
    }
    else
    {
        ui->media_operation_status_label->setText("Add media operation successfull");
        flag = true;
    }
    return flag;
}

bool MainWindow::DeleteMediaByID(const int &media_id)
{
    auto query = QSqlQuery{};
    query.prepare("DELETE FROM media WHERE media.mediaid=?");
    query.addBindValue(media_id);
    auto flag = false;
    if (query.exec())
    {
        flag = true;
        ui->media_operation_status_label->setText("Media deleted successfully");
    }
    else
    {
        qDebug() << query.lastError().text();
        ui->media_operation_status_label->setText("Unable to delete media from DB");
    }
    return flag;
}

void MainWindow::on_delete_media_button_released()
{
    if (ui->media_table_widget->currentRow() >= 0)
    {
        const auto kMediaId = media_ids_[ui->media_table_widget->currentRow()];
        qDebug() << "Media with ID " << kMediaId << " will be deleted, movie ID: " << current_movie_id_;
        if (DeleteMediaByID(kMediaId))
        {
            UpdateMediaList(current_movie_id_);
        }
    }
}
