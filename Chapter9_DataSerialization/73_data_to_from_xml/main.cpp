/**
 * @file main.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation is done with Qt 5.11.3 with gcc 9.4.0
 * 
 * This file is solution to "Problem 73. Serializing and deserializing data to/from XML"
 *  mentioned in "Chapter 9 : Data Serialization" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * This problem is solved in Qt because I am more familiar with Qt than any other
 * libraries or frameworks. Also Qt provides good documentation which enables to
 * develop solutions faster.
 * 
 * The provided code revolves around the representation, processing, and serialization
 * of movie data in XML format. Here is a summary of the relevant parts:
 * - Movie and ActorCast Structures:
 *      Movie structure holds information about a movie, including ID, title, release year,
 *      duration, directors, writers, and cast. ActorCast structure represents the real and
 *      movie names of an actor in a specific movie.
 * - Stream Insertion Operator for Movie:
 *      Overloaded << operator allows streaming movie details to an output stream in a
 *      formatted manner.
 * - WriteXmlDataToStream Function Template:
 *      The function template WriteXmlDataToStream facilitates writing XML data for
 *      vectors of various types, including directors, writers, and cast.
 * - GetXmlString Function:
 *      GetXmlString generates an XML string representation for a vector of Movie objects.
 *      It utilizes the WriteXmlDataToStream function template for writing XML data for directors,
 *      writers, and cast.
 * - ExtractMoviesInfoFromXml Function:
 *      ExtractMoviesInfoFromXml parses movie information from an XML string using the Qt XML
 *      stream reader (QXmlStreamReader). It constructs a vector of Movie objects from the XML data.
 * 
 * Driver code:
 * - Intializes a list of movies.
 * - Creates a xml string of all movies.
 * - Writes the xml string to a file "movies.xml"
 * - Extract movies info from serialized xml string.
 * - Prints the movies info.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <QCoreApplication>
#include <QXmlStreamReader>
#include <QDebug>

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iterator>
#include <algorithm>

using std::cbegin;
using std::cend;
using std::copy;
using std::cout;
using std::chrono::minutes;
using std::endl;
using std::ofstream;
using std::ostream_iterator;
using std::ostream;
using std::quoted;
using std::string;
using std::string_view;
using std::stringstream;
using std::to_string;
using std::vector;

using namespace std::chrono_literals;

/**
 * @brief Struct representing the cast of an actor in a movie.
 */
struct ActorCast
{
    string real_name; /* Real name of the actor. */
    string movie_name;/* Name of the movie character played by the actor.*/
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

/**
 * @brief Overloaded stream insertion operator for printing Movie information to an ostream.
 */
ostream& operator<<(ostream &out, const Movie &movie)
{
    /* Printing movie details in a formatted manner*/
    out << "id        : " << movie.id                     << '\n';
    out << "title     : " << movie.title                  << '\n';
    out << "year      : " << movie.release_year           << '\n';
    out << "minutes   : " << movie.length_minutes.count() << '\n';
    /* Printing directors' names*/
    out << "directors :\n\t";
    copy(cbegin(movie.directors), cend(movie.directors) - 1, ostream_iterator<string>{ out, "\n\t" });
    out << movie.directors.back() << '\n';
    /* Printing Writers' names*/
    out << "writers   :\n\t";
    copy(cbegin(movie.writers), cend(movie.writers) - 1, ostream_iterator<string>{ out, "\n\t" });
    out << movie.writers.back() << '\n';
    /* Printing casting details*/
    out << "cast      :\n";
    for (const auto &[kRealName, kReelName] : movie.start_cast)
    {
        out << "\t" << quoted(kRealName) << " as " << quoted(kReelName) << '\n';
    }

    return out;
}

using MovieList = vector<Movie>;

/**
 * @brief Function template to write XML data to a stringstream.
 * 
 * @tparam T Type of elements in the vector.
 * @tparam XmlDataGenerator Callable object for generating XML data for each element.
 * @param ss Reference to the stringstream.
 * @param field XML field name.
 * @param data Vector of elements.
 * @param generator Callable object generating XML data for each element.
 */
template<class T, class XmlDataGenerator>
void WriteXmlDataToStream(stringstream &ss, string_view field, const vector<T> &data, XmlDataGenerator generator)
{
    ss << "<" << field << ">";
    for (const auto &elem : data)
    {
        generator(ss, elem);
    }
    ss << "</" << field << ">";
}

/**
 * @brief Generates an XML string from a vector of Movie objects.
 * 
 * @param movies_data Vector of Movie objects.
 * @return string XML representation of the movies data.
 */
string GetXmlString(const MovieList &movies_data)
{
    auto ss = stringstream{};
    ss << R"(<?xml version="1.0"?>)";
    ss << "<movies>";
    for (const auto &movie : movies_data)
    {
        ss << "<movie"
            << " id=" << quoted(to_string(movie.id)) << " title=" << quoted(movie.title)
            << " year=" << quoted(to_string(movie.release_year)) << " length=" << quoted(to_string(movie.length_minutes.count()))
            << ">";
            /* Writing XML data for directors, writers, and cast using the WriteXmlDataToStream function template*/
            WriteXmlDataToStream(ss, "directors", movie.directors, [](ostream &out, const string &director) {
                out << "<director name="<< quoted(director) << "/>";
            });
            WriteXmlDataToStream(ss, "writers", movie.writers, [](ostream &out, const string &writer) {
                out << "<writer name="<< quoted(writer) << "/>";
            });
            WriteXmlDataToStream(ss, "cast", movie.start_cast, [](ostream &out, const ActorCast &actor_cast){
                out << "<role actor=" << quoted(actor_cast.real_name) << " role=" << quoted(actor_cast.movie_name) << "/>";
            });

        ss << "</movie>";
    }
    ss << "</movies>";
    return ss.str();
}

/**
 * @brief Extracts movie information from an XML string.
 * 
 * @param movies_xml XML string containing movie information.
 * @return MovieList Vector of Movie objects extracted from the XML.
 */
MovieList ExtractMoviesInfoFromXml(string_view movies_xml)
{
    auto xml_reader = QXmlStreamReader(movies_xml.data());
    auto movies     = MovieList{};
    auto movie      = Movie{};
    while (false == xml_reader.atEnd())
    {
        auto token = xml_reader.readNext();
        if (QXmlStreamReader::StartDocument == token)
        {
            cout << "Document started" << endl;
        }
        else if (QXmlStreamReader::StartElement == token)
        {
            const auto kElementName = xml_reader.name();
            auto attributes = xml_reader.attributes();
            if ("movie" == kElementName)
            {
                movie.id             = attributes.value("id").toULongLong();
                movie.title          = attributes.value("title").toString().toStdString();
                movie.length_minutes = minutes{ attributes.value("length").toInt() };
                movie.release_year   = attributes.value("year").toInt();
            }
            else if ("director" == kElementName)
            {
                movie.directors.push_back(attributes.value("name").toString().toStdString());
            }
            else if ("writer" == kElementName)
            {
                movie.writers.push_back(attributes.value("name").toString().toStdString());
            }
            else if ("role" == kElementName)
            {
                const auto kActor     = attributes.value("actor").toString().toStdString();
                const auto kMovieName = attributes.value("role").toString().toStdString();
                movie.start_cast.push_back(ActorCast{ kActor, kMovieName });
            }
        }
        else if (QXmlStreamReader::EndElement == token)
        {
            if ("movie" == xml_reader.name())
            {
                movies.push_back(movie);
                movie = Movie{};
            }
        }
    }
    return movies;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    auto movies = MovieList{
           {
               1, "Batman Begins", 2005, 140min,
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
            {
                4, "Mission Impossible : Dead Reckoning Part 1", 2023, 163min,
                { "Christopher McQuarrie" },
                { "Christopher McQuarrie" },
                {
                    { "Tom Cruise"      , "Ethan Hunt"        },
                    { "Hayley Atwell"   , "Grace"             },
                    { "Vanessa Kirby"   , "Alanna Mitsopolis" },
                    { "Rebecca Ferguson", "Ilsa Faust"        },
                    { "Pom Klementieff" , "Paris"             },
                    { "Esai Morailes"   , "Gabriel"           },
                    { "Simon Pegg"      , "Benji Dunn"        },
                    { "Ving Rhames"     , "Luther Stickell"   }
                }
            },
            {
                5, "John Wick : Chapter 4", 2023, 169min,
                { "Chad Stahelski" },
                { "Shay Hatten", "Michael Finch" },
                {
                    { "Keanu Reeves"      , "John Wick"       },
                    { "Bill Skarsgard"    , "Marquis Vincent" },
                    { "Scot Adkins"       , "Killa"           },
                    { "Donnie Yan"        , "Caine"           },
                    { "Lance Riddick"     , "Charon"          },
                    { "Ian McShane"       , "Winston"         },
                    { "Hiroyuki Sanada"   , "Shimazu Koji"    },
                    { "Shamier Anderson"  , "Tracker"         },
                    { "Laurence Fishburne", "Bowery King"     }
                }
            }
    };


    const auto kSerialized = GetXmlString(movies);
    if (auto fout = ofstream{ "movies.xml" }; fout.is_open())
    {
        fout << kSerialized;
    }
    cout << kSerialized << endl;

    auto kMoviesExtracted = ExtractMoviesInfoFromXml(kSerialized);
    copy(cbegin(kMoviesExtracted), cend(kMoviesExtracted), ostream_iterator<Movie>{ cout, "\n" });

    return 0;
}
