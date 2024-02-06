#ifndef MOVIE_H
#define MOVIE_H

#include <chrono>
#include <vector>
#include <iterator>
#include <algorithm>

using std::chrono::minutes;
using std::vector;

template <class StringType>
struct ActorCast
{
    StringType real_name;
    StringType movie_name;
};

template <class StringType>
struct Movie
{
    using str_type = StringType;
    size_t            id;
    StringType        title;
    int               release_year;
    minutes           length_minutes;
    vector<StringType>directors;
    vector<StringType>writers;
    vector<ActorCast<StringType>> star_cast;
};


#endif // MOVIE_H
