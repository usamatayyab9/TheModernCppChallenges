/**
 * @file 53_average_rating_of_movies.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 53_average_rating_of_movies.cpp
 *  
 * This file is solution to "Problem 53. Average rating of movies"
 * mentioned in "Chapter 6: Algorithms and Data Structures" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * A structure `MovieInfo` is provided for representing movie name and its ratings.
 * Another structure `AverageCalculator` is provided for calculating average rating of a movie.
 * 
 * Driver code:
 * - Initalizes a list of movies
 * - Iterates over the list of movies and prints each movie's name and its computed rating.
 *  
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <cmath>
#include <iomanip>

using std::accumulate;
using std::begin;
using std::cbegin;
using std::cend;
using std::ceil;
using std::cout;
using std::end;
using std::endl;
using std::size;
using std::sort;
using std::string;
using std::vector;

struct MovieInfo
{
    string name;
    vector<float> ratings;
};

/**
 * @brief A struct for calculating average of a movie.
 * 
 */
struct AverageCalculator
{
    int percentage{ 5 };
    
    /**
     * @brief Calculates the average rating of @param movie_info
     * - Calculates the elements to discard from calculations, i.e.
     *      5% of total ratings
     * - Sort ratings vector.
     * - Calculates the sum of all movies discarding elements from start
     *  and from end.(No of elements to discard is calculated in step 1).
     * - returns the average.
     * 
     * @param movie_info 
     * @return float 
     */
    float operator()(const MovieInfo &movie_info)
    {
        auto ratings            = movie_info.ratings;
        const auto kRemoveCount = std::ceil( (size(ratings) * percentage) / 100.0f );
        
        sort(begin(ratings), end(ratings));
        
        const auto kSumOfRatings = accumulate(
            cbegin(ratings) + kRemoveCount, cend(ratings) - kRemoveCount, 0.0f
        );

        return kSumOfRatings / (size(ratings) - 2 * kRemoveCount);
    }
};

int main()
{
    auto movies = vector<MovieInfo>
    {
        { "The Matrix", { 10, 9, 10, 9, 9, 8, 7, 10, 5, 9, 9, 8 } },
        { "Gladiator", { 10, 5, 7, 8, 9, 8, 9, 10, 10, 5, 9, 8, 10 } },
        { "Interstellar", { 10, 10, 10, 9, 3, 8, 8, 9, 6, 4, 7, 10 } }
    };

    auto mean_calculator = AverageCalculator{};
    for (const auto kMovieInfo : movies)
    {
        cout << "Name: " << kMovieInfo.name << " , Rating: " << std::fixed << std::setprecision(1) << mean_calculator(kMovieInfo) << endl;
    }

    return 0;
}
