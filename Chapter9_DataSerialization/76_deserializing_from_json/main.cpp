/**
 * @file main.cpp
 * @author Usama Tayyab (usamtayyab9@@gmail.com)
 * @brief 
 * Compiled with Qt 5.11.3 using gcc 9.4.0
 * 
 * This file is solution to "Problem 76. Deserializing data from JSON"
 *  mentioned in "Chapter 9 : Data Serialization" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * - ActorCast:
 *      Represents an actor's real name and the corresponding movie name.
 * - Movie:
 *      Contains information about a movie, including its ID, title,
 *      release year, duration, directors, writers, and the cast.
 * - MovieList:
 *      An alias for a vector of movies.
 * - Functionality:
 *      Overloaded output stream operator (operator<<) is provided for
 *      printing movie information.
 * - ReadFile:
 *      Reads the content of a file specified by the filename into a
 *      QByteArray and returns it.
 * - TransfromJsonArrayToVector:
 *      Converts a QJsonArray to a vector using a projection function.
 * - JSONArrayToMoviesList:
 *      Converts a QJsonArray of movies to a vector of Movie objects.
 * 
 * Summary:
 *      The code facilitates working with movie data in JSON format.
 *      It defines structures to represent movies and actor casts and includes
 *      functions for reading JSON data from a file and converting JSON arrays
 *      into vectors of Movie objects. The JSON parsing and conversion functions
 *      utilize Qt classes for handling JSON data.
 *
 * Driver code:
 * - Reads the file movies.json using `ReadFile()` function.
 * - Prints movies description if available.
 * - Parse the movies info from json string.
 * - Prints movies info on the console.
 *  
 * @copyright Copyright (c) 2024
 * 
 */
/* QT-Includes */
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include <QString>

/* C++-Includes*/
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <iterator>
#include <algorithm>

using std::cbegin;
using std::cend;
using std::copy;
using std::chrono::minutes;
using std::ostream_iterator;
using std::ostream;
using std::quoted;
using std::string;
using std::vector;

using namespace std::chrono_literals;

struct ActorCast
{
    string real_name;
    string movie_name;
};

struct Movie
{
    size_t            id;
    string            title;
    int               release_year;
    minutes           length_minutes;
    vector<string>    directors;
    vector<string>    writers;
    vector<ActorCast> start_cast;
};

using MovieList = vector<Movie>;

ostream& operator<<(ostream &out, const Movie &movie)
{
    out << "id        : " << movie.id<< '\n';
    out << "title     : " << movie.title << '\n';
    out << "year      : " << movie.release_year << '\n';
    out << "minutes   : " << movie.length_minutes.count() << '\n';

    out << "directors :\n\t";
    copy(cbegin(movie.directors), cend(movie.directors) - 1, ostream_iterator<string>{ out, "\n\t" });
    out << movie.directors.back() << '\n';

    out << "writers   :\n\t";
    copy(cbegin(movie.writers), cend(movie.writers) - 1, ostream_iterator<string>{ out, "\n\t" });
    out << movie.writers.back() << '\n';

    out << "cast      :\n";
    for (const auto &[kRealName, kReelName] : movie.start_cast)
    {
        out << "\t" << quoted(kRealName) << " as " << quoted(kReelName) << '\n';
    }

    return out;
}

/**
 * @brief Reads the content of a file specified by the filename
 * into a QByteArray and returns it
 * 
 * @param filename - Name of the file to be read
 * @return         - QByteArray containing the file content
 */
QByteArray ReadFile(const QString &filename)
{
    auto byte_array = QByteArray{};
    auto file       = QFile{ filename };
    file.open(QIODevice::Text | QIODevice::ReadOnly);
    byte_array = file.readAll();
    return byte_array;
}

/**
 * @brief
 *      Converts a QJsonArray to a vector using a projection function
 *      Iterates over the elements of param json_array, applies the
 *      projection param proj and stores the projected result into a
 *      vector.
 * 
 * @tparam T Type of elements in the resulting vector
 * @tparam ProjectionFunction Type of the projection function
 * @param json_array - QJsonArray to be converted
 * @param proj       - Projection function to transform each element
 * @return Vector containing projected element
 */
template<class T, class ProjectionFunction>
vector<T> TransfromJsonArrayToVector(const QJsonArray &json_array, ProjectionFunction proj)
{
    auto vec = vector<T>(json_array.size());
    std::transform(cbegin(json_array), cend(json_array), vec.begin(), proj);
    return vec;
}

/**
 * @brief Converts a QJsonArray of movies to a vector of Movie objects
 * @param movies_array QJsonArray containing movie information
 * @return Vector of Movie objects
 */
MovieList JSONArrayToMoviesList(const QJsonArray &movies_array)
{
    auto movies = MovieList{};
    for (const auto kJsonValue : movies_array)
    {
        const auto kJsonMovieObj = kJsonValue.toObject();
        auto movie               = Movie{};
        movie.id                 = kJsonMovieObj["id"].toInt(-1);
        movie.title              = kJsonMovieObj["title"].toString("").toStdString();
        movie.release_year       = kJsonMovieObj["year"].toInt(-1);
        movie.length_minutes     = minutes{ kJsonMovieObj["length"].toInt(-1) };
        movie.directors          = TransfromJsonArrayToVector<string>(kJsonMovieObj["directors"].toArray(),
        [](const QJsonValue &kDirector) {
            return kDirector.toString("").toStdString();
        });
        movie.writers            = TransfromJsonArrayToVector<string>(kJsonMovieObj["writers"].toArray(),
        [](const QJsonValue &kWriter) {
            return kWriter.toString("").toStdString();
        });
        movie.start_cast         = TransfromJsonArrayToVector<ActorCast>(kJsonMovieObj["cast"].toArray(),
        [](const QJsonValue &star_cast) {
            return ActorCast{ star_cast["actor"].toString("").toStdString(), star_cast["role"].toString("").toStdString() };
        });
        movies.push_back(movie);
    }

    return movies;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    auto json_error = QJsonParseError{};
    if (auto json_doc = QJsonDocument::fromJson(ReadFile("../movies.json"), &json_error); QJsonParseError::NoError == json_error.error)
    {
        auto json_object = json_doc.object();
        if (json_object.contains("description"))
        {
            qDebug() << json_object["description"].toString("No Description Available");
        }
        if (json_object.contains("movies") && json_object["movies"].isArray())
        {
            const auto kMovies = JSONArrayToMoviesList(json_object["movies"].toArray());
            copy(cbegin(kMovies), cend(kMovies), ostream_iterator<Movie>{ std::cout, "\n"});
        }
    }

    return 0;
}
