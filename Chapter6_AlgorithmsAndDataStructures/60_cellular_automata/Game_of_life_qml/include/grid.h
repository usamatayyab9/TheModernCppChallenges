/**
 * @file grid.h
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef GRID_H
#define GRID_H
# include <valarray>
# include <iostream>
# include <tuple>

using std::cout;
using std::valarray;
using std::slice;
using std::gslice;
using std::slice_array;
using std::pair;

class Grid
{
    public:
    Grid()=default;
    Grid(valarray<int> v,size_t r,size_t c);
    Grid(size_t r,size_t c,int val=int());
    std::pair<size_t,size_t> dimension() const;

    /*return element at row:i and column:j. no range check is performed*/
    const int& operator()(size_t i, size_t j)const;
    int& operator()(size_t i,size_t j);
    const int &operator()(size_t i)const;
    int &operator()(size_t i);
    const int& operator[](const pair<size_t,size_t> &p)const;
    int& operator[](const pair<size_t,size_t> &p);
    int& at(size_t i,size_t j);

    /*total number of elements in grid*/
    size_t size() const;

    /*return number of rows*/
    size_t size1() const;
    size_t dim1() const;
    size_t rows() const;

    /*return number of columns*/
    size_t size2() const;
    size_t dim2() const;
    size_t columns() const;

    /*returns reference to n-th row grid. to get single dimensional vector(valarray)  apply VA function. No range checking*/
    slice_array<int> row(size_t n);
    const valarray<int> row(size_t n) const;

    /*returns reference to n-th column of grid. to get single dimensional vector(valarray)  apply VA function. No range checking*/
    slice_array<int> column(size_t n);
    const valarray<int> column(size_t n) const;

    void setAll(int b);

private:
    valarray<int> data;
    size_t mrows{ 0 }, mcols{ 0 };
};
void print(const Grid &m);
size_t rows(const Grid &g);
size_t columns(const Grid &g);
size_t size(const Grid &g);
size_t size1(const Grid &g);
size_t size2(const Grid &g);

#endif // GRID_H
