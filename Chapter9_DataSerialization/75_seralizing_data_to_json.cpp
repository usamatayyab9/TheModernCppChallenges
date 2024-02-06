/**
 * @file 75_seralizing_data_to_json.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * 
 * Compilation command : g++ -std=c++17 75_seralizing_data_to_json.cpp
 * 
 * This file is solution to "Problem 75. Serializing data to JSON"
 *  mentioned in "Chapter 9 : Data Serialization" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * 
 * - Structures:
 *      ActorCast: Represents an actor's real name and the corresponding movie name.
 * - Movie:
 *      Contains information about a movie, including its ID, title, release year,
 *      duration, directors, writers, and the cast.
 * - MovieList:
 *      An alias for a vector of movies.
 * - Functionality:
 *      Overloaded output stream operator (operator<<) is provided for printing
 *      movie information.
 * - `MoviesDataJSONSerializer` Class:
 *      Serializes movie data into JSON format. Provides functions to generate
 *      JSON arrays and the overall JSON representation for a movie. Includes a
 *      templated function GenrateJSONArray to generate JSON arrays for different
 *      types of vectors. The class can generate JSON arrays for vectors of strings,
 *      ActorCast objects, and the overall JSON representation for a Movie object.
 *      The function call operator is overloaded to serialize a list of movies
 *      into a JSON string.
 * - Summary:
 *      The code defines data structures for movies, including actors and their roles,
 *      and introduces a serializer class (MoviesDataJSONSerializer) to convert this
 *      movie data into JSON format. The class supports the generation of JSON arrays
 *      for various types and produces a JSON string for a list of movies.
 * 
 * Driver code:
 * - Initializes a list of movies.
 * - Creates a json string representation of list of movies.
 * - Writes the json to a file movies.json
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <functional>

using std::bind;
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
using std::size;
using std::string;
using std::string_view;
using std::stringstream;
using std::to_string;
using std::vector;

using namespace std::chrono_literals;

/**
 * @brief Represents the data of an actor's real name and the corresponding movie name.
 */
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
 * @brief Overloaded stream insertion operator for printing Movie information to an ostream.
 */
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
 * @brief Serializes movie data into JSON format.
 * This struct provides functions to generate JSON arrays and the overall JSON representation for a movie.
 * The function overloads the function operator so that it can be used a functor too. 
 */
struct MoviesDataJSONSerializer
{
    /**
     * @brief 
     * Formats all elements of @param vec into json format, such that elements are separated by
     * comma(,). Wherther to include quotes around each element in json is controlled by @param include_quotes.
     * JSON representation for each element is obtained by passing the element into @param json_generator
     * 
     * @tparam          DataType 
     * @tparam          JSONGenrator 
     * @param   vec   Vector of elements which
     * @param   json_generator 
     * @param   include_quotes - Control whether double should be added around fields 
     * @return  string - The generated JSON as a string
     */
    template <class DataType, class JSONGenrator>
    string GenrateJSONArray(const vector<DataType> &vec, JSONGenrator json_generator, bool include_quotes)
    {
        auto ss = stringstream{};
        for (const auto &kData : vec)
        {
            if (include_quotes) { ss << quoted(json_generator(kData)) << ","; }
            else                { ss << json_generator(kData) << ","; }
        }
        auto str = ss.str();
        if (size(str) > 0 && ',' == str.back())
        {
            str.pop_back();
        }
        return str;
    }

    /**
     * @brief Generates a JSON array for a vector of strings.
     * 
     * @param vec - Vector of strings.
     * @return string The generated JSON array as a string.
     */
    string GenerateJSONArray(const vector<string> &vec)
    {
        return GenrateJSONArray(vec, [](const auto &str){ return str; }, true);
    }

    /**
     * @brief Generates a JSON array for a vector of ActorCast objects.
     * 
     * @param vec - Vector of ActorCast objects.
     * @return string The generated JSON array as a string.
     */
    string GenerateJSONArray(const vector<ActorCast> &vec)
    {
        auto ss = stringstream{};
        return GenrateJSONArray(vec, [&ss](const ActorCast &actor_cast){
            ss.str("");
            ss << '{'
                << quoted("actor") << ":" << quoted(actor_cast.real_name) << ","
                << quoted("role") << ":" << quoted(actor_cast.movie_name) 
            << "}";
            return ss.str();
        },
        false);
    }
    
    /**
     * @brief Generates the JSON representation for a Movie object.
     * 
     * @param movie The Movie object to be serialized.
     * @return string The generated JSON representation as a string.
     */
    string GetJSONForMovie(const Movie &movie)
    {
        auto sstream = stringstream{};
        sstream << '{';
        sstream << quoted("id")         << ":" << movie.id                                   << ",";
        sstream << quoted("title")      << ":" << quoted(movie.title)                        << ",";
        sstream << quoted("year")       << ":" << movie.release_year                         << ",";
        sstream << quoted("length")     << ":" << movie.length_minutes.count()               << ",";
        sstream << quoted("directors")  << ":" << "[" << GenerateJSONArray(movie.directors)  << "]" << ",";
        sstream << quoted("writers")    << ":" << "[" << GenerateJSONArray(movie.writers)    << "]" << ",";
        sstream << quoted("cast")       << ":" << "[" << GenerateJSONArray(movie.start_cast) << "]" ;
        sstream << '}';
        return sstream.str();
    }

    /**
     * @brief Function call operator to serialize a list of movies into a JSON string.
     * 
     * @param movies_data The list of movies to be serialized.
     * @return string The generated JSON string.
     */
    string operator()(const MovieList &movies_data)
    {
        auto sstream = stringstream{};
        sstream << '{' << quoted("description") << ":" << quoted("json for movies") << ",";
        sstream << quoted("movies") << ":"
        << '['
            << GenrateJSONArray(movies_data, bind(&MoviesDataJSONSerializer::GetJSONForMovie, this, std::placeholders::_1), false)
        << ']';
        sstream << '}';
        return sstream.str();
    }
};

int main()
{
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
        }
    };

    auto json_serializer    = MoviesDataJSONSerializer{};
    const auto kMoviesJSON  = json_serializer(movies);
    auto fout               = ofstream{ "movies.json" };
    fout << kMoviesJSON;

    return 0;
}