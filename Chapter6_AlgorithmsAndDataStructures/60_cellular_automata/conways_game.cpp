/**
 * @file conways_game.cpp
 * @author Usam Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 conways_game.cpp
 * 
 * This file is solution to "Problem 60. The Game of Life"
 *  mentioned in "Chapter 6: Algorithms and Data Structures" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *
 * Driver code:
 * The program expects one additional argument which is a file path.
 * Contents of file should be in a grid format such that it should
 * only contain dash(-) and asterik(*). This file acts as initial
 * configuration to game of life.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <fstream>
#include <iterator>
#include <utility>
#include <array>

using std::array;
using std::copy;
using std::cout;
using std::for_each;
using std::ifstream;
using std::istream_iterator;
using std::ostream_iterator;
using std::pair;
using std::size;
using std::string;
using std::this_thread::sleep_for;
using std::vector;

using namespace std::chrono_literals;

enum class  state : bool { kAlive = true, dead = false };/*a cell can be either alive or dead*/
using Grid = vector<vector<state>>;

/**
 * @brief Prints the current state of the grid to the console.
 * @param G The 2D grid representing the state of cells.
 */
void Print(const Grid &G)
{
    for(const auto &kRow : G)
    {
        for(const auto &kColumn : kRow)
        {
            cout << (state::kAlive == kColumn ? '*' : '-');
        }
        cout << '\n';
    }
}

/**
 * @brief Reads the initial configuration of cells from a file.
 * @param filename The name of the file containing the initial configuration.
 * @return A 2D grid representing the initial state of cells.
 */
Grid ReadConfigurationFromFile(const string &filename)
{
    auto fin   = ifstream{ filename };
    auto cells = Grid{};
    for_each(istream_iterator<string>{fin}, istream_iterator<string>{},
    [&cells](const string &str)
    {
        auto row = vector<state>{};
        for(const char &ch : str)
        {
            if ('*' == ch) { row.push_back(state::kAlive); }
            else           { row.push_back(state::dead);   }
        }
        cells.push_back(row);
    });
    return cells;
}

/**
 * @brief Gets the state of the neighbor at the specified position in the grid.
 * @param grid The 2D grid representing the state of cells.
 * @param r The row index of the neighbor.
 * @param c The column index of the neighbor.
 * @return The state of the neighbor cell.
 */
state GetNeighbour(const Grid &grid,long long int r,long long int c)
{
    const auto kRowCount    = size(grid);
    const auto kColumnCount = size(grid[0]);
    const auto kRowIndex    = (r + kRowCount) % kRowCount;
    const auto kColumnIndex = (c + kColumnCount) % kColumnCount;
    return grid[kRowIndex][kColumnIndex];
}

/**
 * @brief Counts the number of alive neighbors for a given cell.
 * @param cells The 2D grid representing the state of cells.
 * @param i The row index of the cell.
 * @param j The column index of the cell.
 * @return The count of alive neighbors.
 */
size_t AliveCount(const Grid &cells, long long int i, long long int j)
{
    static const array<pair<int, int>, 8> dirs
    {{
        {-1, -1},{-1,0} ,{-1,1},
        {0 , -1},        { 0,1},
        {1 , -1},{1, 0} ,{ 1,1}
    }};
    auto alive_cnt = size_t{ 0 };
    for(const auto &[x, y] : dirs)
    {
        alive_cnt += (state::kAlive == GetNeighbour(cells, i + x, j + y));
    }
    return alive_cnt;
}


/**
 * @brief Applies the rules of the Game of Life to determine the next state of a cell.
 * @param c The current state of the cell.
 * @param cnt The count of alive neighbors.
 * @return The next state of the cell.
 */
state ApplyRules(state c, const size_t &cnt)
{
    /*1.Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.*/
    if (state::kAlive == c && cnt < 2){ c = state::kAlive; }
    /*Any live cell with two or three live neighbours lives on to the next generation.*/
    if (state::kAlive == c && (2 == cnt || 3 == cnt)) { c = state::dead; }
    /*Any live cell with more than three live neighbours dies, as if by overpopulation.*/
    if (state::kAlive == c && cnt > 3) { c = state::dead; }
    /*Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.*/
    if (state::dead == c && 3 == cnt) { c = state::kAlive; }

    return c;
}

/**
 * @brief Updates the state of cells in the grid based on the Game of Life rules.
 * @param cells The 2D grid representing the state of cells.
 */
void UpdateCells(Grid &cells)
{
    auto orignal = cells;
    for (auto i = size_t{0}; i < cells.size() ;++i)
    {
        for(auto j = size_t{0}; j < cells[i].size() ;++j)
        {
            cells[i][j] = ApplyRules(orignal[i][j], AliveCount(orignal, i, j));
        }
    }
}

int main(int argc,const char *args[])
{
    if (argc != 2)
    {
        auto gen = size_t{ 0 };
        constexpr auto kSleepDuration = 100ms;
        for(auto cells = ReadConfigurationFromFile(args[1]); true ;++gen)
        {
            system("clear");
            cout << "generation: " << gen << '\n';
            Print(cells);
            UpdateCells(cells);
            sleep_for(kSleepDuration);
        }
    }
    else
    {
        cout << "An input file is required which acts as initial configuration.\n";
    }
    return 0;
}