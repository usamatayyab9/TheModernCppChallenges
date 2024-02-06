/**
 * @file main.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compiled with Qt 5.11.3 with gcc 9.4.0
 * 
 * This file is solution to "Problem 77. Printing a list of movies to a PDF"
 *  mentioned in "Chapter 9 : Data Serialization" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * - ActorCast:
 *      A struct that represents an actor's real name and the corresponding movie name.
 * - Movie:
 *      A struct that Contains information about a movie, including its ID, title,
 *      release year, duration, directors, writers, and the cast.
 * 
 * To solve this problem I used Qt for writing to a pdf file. There are many other libraries
 * for PDF writing but I am most familiar with Qt so I went with Qt QPdfWriter class. See
 * `WriteMoviesDataToPDF()` for more details.
 * 
 * Driver code:
 * Initializes a list of movies.
 * Writes them to a file movies.pdf
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <QCoreApplication>
#include <QPdfWriter>
#include <QPainter>
#include <QRect>
#include <QString>
#include <QGuiApplication>
#include <QPen>
#include <QPageSize>

#include <string>
#include <vector>
#include <chrono>


using std::chrono::minutes;
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

/**
 * @brief Writes information about a list of movies to a PDF file.
 * @details This function generates a PDF document containing details
 *          such as movie title, release year, and duration for each
 *          movie in the provided list. Each page of the PDF can
 *          accommodate up to 25 movies, and a new page is started when
 *          this limit is reached.
 * 
 * @param filename Name of the PDF file to be created.
 * @param movies List of movies to be written to the PDF.
 * 
 * @note All of the constants in this function are found by hit and trial.
 */
void WriteMoviesDataToPDF(const QString &filename, const MovieList &movies)
{
    /*Create a PDF writer with the specified filename and set the page size to A4*/
    QPdfWriter writer(filename);
    writer.setPageSize(QPageSize(QPageSize::A4));

    QPainter painter(&writer);
    painter.setFont(QFont("Arial", /*Point size*/20, /*weight of font*/1000));
    auto pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);

    painter.drawText(10, 24, writer.width(), 250, Qt::AlignCenter, "List of Movies");
    painter.drawLine(QPoint(10, 500), QPoint(writer.width() - 10, 500));
    painter.setFont(QFont("Arial", 15));

    auto y_coord        = 700;
    auto movies_wrriten = 0;
    for (const auto &movie : movies)
    {
        if (movies_wrriten >= 25) /*After 25 movies move to new page*/
        {
            painter.drawLine(QPoint(10, y_coord), QPoint(writer.width() - 10, y_coord)); // Draw line at end of page
            writer.newPage(); // Inser new page
            painter.drawLine(QPoint(10, 500), QPoint(writer.width() - 10, 500)); // Draw line at start of new page
            movies_wrriten = 0;
            y_coord        = 700;
        }
        auto str = QString{ "%1 (%2)" }.arg(QString::fromStdString(movie.title)).arg(movie.release_year);
        painter.drawText(10, y_coord, writer.width(), 500, Qt::AlignLeft, str);

        auto hours             = std::chrono::duration_cast<std::chrono::hours>(movie.length_minutes);
        auto remaining_minutes = movie.length_minutes - std::chrono::duration_cast<std::chrono::minutes>(hours);
        str = QString{ "%1:%2" }.arg(hours.count()).arg(remaining_minutes.count(), 2, 10, QChar{'0'});
        painter.drawText(10 + writer.width() / 2, y_coord, writer.width() / 2, 500, Qt::AlignRight, str);

        y_coord += 500;
        ++movies_wrriten;
    }
    painter.drawLine(QPoint(10, y_coord), QPoint(writer.width() - 10, y_coord));
}

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    auto movies = MovieList{
        {
            1, "Batman Begins", 2005, 120min,
            { "Christopher Nolan" },
            { "Christopher Nolan", "David S. Goyer" },
            {
                { "Christain Bale", "The Batman"     },
                { "Cillian Murphy", "Scarecrow"      },
                { "Katie Holmes"  , "Rachel Dawes"   },
                { "Michael Caine" , "Alfred"         },
                { "Liam Neeson"   , "Ra's Al Ghul"   },
                { "Gary Oldman"   , "Jim Gordon"     },
                { "Morgan Freeman", "Lucious Fox"    }
            }
        },
        {
            2, "Batman The Dark Knight", 2008, 152min,
            { "Christopher Nolan" },
            { "Christopher Nolan" },
            {
                { "Christain Bale", "The Batman"             },
                { "Heath Ledger"  , "Joker"                  },
                { "Aaron Eckhart" , "HarveyD Dent/Two-face"  },
                { "Cillian Murphy", "Scarecrow"              },
                { "Michael Caine" , "Alfred"                 },
                { "Liam Neeson"   , "Ra's Al Ghul"           },
                { "Gary Oldman"   , "Jim Gordon"             },
                { "Morgan Freeman", "Lucious Fox"            }
            }
        },
        {
            3, "Batman The Dark Knight Rises", 2012, 165min,
            { "Christopher Nolan" },
            { "Christopher Nolan", "David S. Goyer" },
            {
                { "Christain Bale"   , "The Batman"           },
                { "Tom Hardy"        , "Bane"                 },
                { "Joseph Gordon"    , "John Blake"           },
                { "Anne Hathaway"    , "Selina Kyle/Catwoamn" },
                { "Marion Cotillard" , "Talia al-ghul"        },
                { "Cillian Murphy"   , "Scarecrow"            },
                { "Michael Caine"    , "Alfred"               },
                { "Liam Neeson"      , "Ra's Al Ghul"         },
                { "Gary Oldman"      , "Jim Gordon"           },
                { "Morgan Freeman"   , "Lucious Fox"          }
            }
        },
        { 4, "Captain America: The First Avenger", 2011, 124min },
        { 5, "Captain America: The Winter Soldier", 2014, 136min },
        { 6, "Captain America: Civil War", 2016, 148min },
        { 7, "Andhadhun", 2018, 139min },
        { 8, "The Lion King", 1994, 87min },
        { 9, "Kung Fu Panda", 2008, 92min },
        { 10, "Justice League Doom", 2012, 77min },
        { 11, "Batman vs. Robin", 2015, 80min },
        { 12, "Son of Batman", 2014, 74min },
        { 13, "Batman: Bad Blood", 2016, 72min },
        { 14, "Batman: Under the Red Hood", 2015, 75min },
        { 15, "Batman: Assault on Arkham", 2014, 75min },
        { 16, "Batman: Hush", 2019, 91min },
        { 17, "Batman: The Killing Joke", 2016, 86min },
        { 18, "Drishyam", 2015, 163min },
        { 19, "The Revenant", 2015, 156min },
        { 20, "Mission Impossible: Dead Reckoning - Part 1", 2023, 163min },
        { 21, "The Grey", 2011, 116min },
        { 22, "Taken", 2008, 90min },
        { 23, "John Wick", 2014, 101min },
        { 24, "John Wick: Chapter 2", 2017, 122min },
        { 25, "John Wick: Chapter 3 - Parebellum", 2019, 131min },
        { 26, "John Wick: Chapter 4", 2023, 169min },
        { 27, "Dangal", 2016, 161min },
        { 28, "3 idiots", 2009, 171min }
    };

    WriteMoviesDataToPDF("movies.pdf", movies);

    return 0;
}
