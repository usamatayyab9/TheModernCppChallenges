/**
 * @file 32_pascals_triangle.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 *  Compilation command : g++ -std=c++17 32_pascals_triangle.cpp
 * This file is solution to "Problem 32. Pascal's triangle"
 *  mentioned in "Chapter 4: Streams and Filesystems" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a function that prints up to 10 rows of Pascal's triangle to the console.
 * 
 * Solution :
 * For information on pascal's triangle visit https://en.wikipedia.org/wiki/Pascal%27s_triangle
 * 
 * This file utilises two items for implementing the complete solution.
 * - A function PascalTriangle() which generates the rows of pascal's triangle. See function comments
 *      for more details.
 * - A struct `PascalsTrianglePrettyPrinter` which provides prints the pascal's triangle in pretty format.
 *      A pretty format is displayed as:
 *                                              1
 *                                             1 1
 *                                            1 2 1
 *                                           1 3 3 1
 *                                          1 4 6 4 1
 *      The stuct PascalsTrianglePrettyPrinter overloads the function operator whic takes a vector<vector>
 *      as argument and prints its elements in the above format. See comments for more details.
 * 
 * Driver code:
 * The program uses above function and struct to generate and print pascal's triangle upto 10 rows.
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include <cmath>

using std::cin;
using std::copy;
using std::cout;
using std::endl;
using std::floor;
using std::for_each;
using std::ostream;
using std::ostream_iterator;
using std::plus;
using std::transform;
using std::vector;

/**
 * @brief Generates Pascal's Triangle up to the specified number of rows.
 * 
 * @param N - Number of rows upto which pascal's triangle will be generated
 * @return vector<vector<size_t>> - representing rows of pascal's triangle
 */
auto PascalTriangle(size_t N)
{
    auto pascal_triangle_rows = vector<vector<size_t>>{};
    if (N >= 1)
    {
        pascal_triangle_rows.reserve(N);
        pascal_triangle_rows.push_back({ 1 });//Insert first row initally
        for (auto idx = size_t{ 1 }; idx < N ;++idx)
        {
            const auto &prev_row = pascal_triangle_rows[idx - 1];
            auto next_row        = vector<size_t>(size(prev_row) + 1);
            next_row.front()     = 1;
            next_row.back()      = 1;
            transform(cbegin(prev_row) + 1, cend(prev_row), cbegin(prev_row), begin(next_row) + 1, plus{});
            pascal_triangle_rows.push_back(next_row);
        }
    }
    return pascal_triangle_rows;
}

/**
 * @brief A functor for printing Pascal's Triangle in a visually aligned and readable format.
 * 
 * This functor takes a vector of vectors representing Pascal's Triangle and prints it in a way that
 * each number is visually aligned, making the triangle more readable. It calculates the maximum
 * number of digits in the triangle to determine the spacing between numbers for proper alignment.
 * 
 * Usage:
 * ```
 * PascalsTrianglePrettyPrinter printer;
 * printer(triangle);
 * ```
 */
struct PascalsTrianglePrettyPrinter
{
    /**
     * @brief Prints Pascal's Triangle to the specified output stream.
     * 
     * This function prints Pascal's Triangle in a visually aligned format. It calculates the maximum
     * number of digits in the triangle to determine the spacing between numbers for proper alignment.
     * 
     * @param triangle - A vector of vectors representing Pascal's Triangle.
     * @return ostream& - A reference to the output stream where the triangle is printed.
     */
    ostream& operator()(const vector<vector<size_t>> &triangle)
    {
        const auto kLenghtOfEachNumber = MaxNumberOfDigits(triangle);
        auto no_of_spaces_before       = (size(triangle) - 1) * kLenghtOfEachNumber;
        for (const auto &row : triangle)
        {
            PrintNSpaces(no_of_spaces_before);
            for (const auto &elem : row) { PrintNumber(elem, kLenghtOfEachNumber); }
            cout << '\n';
            no_of_spaces_before -= kLenghtOfEachNumber;
        }
        return cout;
    }

private:
    void PrintNumber(const size_t &N, const size_t &kMaxLenghtOfEachNumber)
    {
        const auto kLengthOfElem = DigitCount(N);
        /*! Equalize N to kMaxLenghtOfEachNumber,
            if N == kMaxLenghtOfEachNumber, no spaces will be printed
            if N < kMaxLenghtOfEachNumber, kMaxLenghtOfEachNumber - kLengthOfElem spaces will be printed
            It is responsibility of the caller to ensure that N can not be greater than kMaxLenghtOfEachNumber
        */
        PrintNSpaces(kMaxLenghtOfEachNumber - kLengthOfElem); 
        cout << N;
        PrintNSpaces(kMaxLenghtOfEachNumber);
    }

    void PrintNSpaces(const size_t &N)
    {
        for (auto ii = size_t{ 0 }; ii < N ;++ii) { cout << ' '; }
    }

    size_t DigitCount(const int &i)
    {
        return 1 + floor(log10(i));
    }

    size_t MaxNumberOfDigits(const vector<vector<size_t>> &triangle)
    {
        auto max_digit_count = size_t{ 1 };
        for (const auto &row : triangle) {
            for (const auto &elem : row) {
                max_digit_count = std::max(max_digit_count, DigitCount(elem));
            }
        }
        return max_digit_count;
    }
};


int main()
{
    auto n = size_t{ 10 };
    const auto kPascalTriangleRows = PascalTriangle(n);
    PascalsTrianglePrettyPrinter{}(kPascalTriangleRows);
    return 0;
}