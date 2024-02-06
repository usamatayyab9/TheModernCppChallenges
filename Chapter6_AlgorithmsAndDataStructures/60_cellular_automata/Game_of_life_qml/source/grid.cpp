/*compiled with: g++ -std=c++17 grid.h
only compiler that supports c++17 should compile this file*/
#include "grid.h"
    Grid::Grid(valarray<int> v,size_t r,size_t c):
        data{ v },
        mrows{ r },
        mcols{ c }
    {
    }

    Grid::Grid(size_t r,size_t c,int val):
        data(val, r * c),
        mrows{ r },
        mcols{ c }
    {
    }

    std::pair<size_t,size_t> Grid::dimension() const
    {
        return {mrows, mcols};
    }
    /*return element at row:i and column:j. no range check is performed*/
    const int& Grid::operator()(size_t i, size_t j)const
    {
        return data[i * mcols + j];
    }
    int& Grid::operator()(size_t i, size_t j)
    {
        return data[i * mcols + j];
    }

    const int &Grid::operator()(size_t i) const
    {
        return data[i];
    }

    int &Grid::operator()(size_t i)
    {
        return data[i];
    }
    const int& Grid::operator[](const pair<size_t,size_t> &p)const
    {
        return data[p.first * mcols + p.second];
    }
    int& Grid::operator[](const pair<size_t,size_t> &p)
    {
        return data[p.first * mcols + p.second];
    }
    int& Grid::at(size_t i,size_t j)
    {
        if (i >= mrows || j >= mcols) { throw std::out_of_range("Invalid argument"); }
        return this->operator()(i, j);
    }
    /*total number of elements in grid*/
    size_t Grid::size() const
    {
        return data.size();
    }
    /*return number of rows*/
    size_t Grid::size1() const
    {
        return mrows;
    }
    size_t Grid::dim1() const
    {
        return size1();
    }
    /*return number of columns*/
    size_t Grid::size2() const
    {
        return mcols;
    }
    size_t Grid::dim2() const
    {
        return size2();
    }
    size_t Grid::rows() const
    {
        return size1();
    }
    size_t Grid::columns() const
    {
        return size2();
    }

    /*returns reference to n-th row grid. boolo get single dimensional vector(valarray)  apply VA function. No range checking*/
    slice_array<int> Grid::row(size_t n)
    {
        return data[slice(n * mcols, mcols, 1)];
    }
    const valarray<int> Grid::row(size_t n) const
    {
        return data[slice(n * mcols, mcols, 1)];
    }

    /*returns reference to n-th column of grid. boolo get single dimensional vector(valarray)  apply VA function. No range checking*/
    slice_array<int> Grid::column(size_t n)
    {
        return data[std::slice(n, mrows, mcols)];
    }
    const valarray<int> Grid::column(size_t n) const
    {
        return data[std::slice(n, mrows, mcols)];
    }
    void Grid::setAll(int b)
    {
        data = b;
    }

    //non-member functions
void print(const Grid &m)
{
    const auto [kRowCount, kColumnCount] = m.dimension();
    for (auto i = size_t{0}; i < kRowCount ;++i)
    {
        for (auto j = size_t{0}; j < kColumnCount ;++j) { cout << m(i,j) << " "; }
        cout << '\n';
    }
}
size_t rows(const Grid &g)
{
    return g.size1();
}
size_t columns(const Grid &g)
{
    return g.size2();
}
size_t size(const Grid &g)
{
    return g.size();
}
size_t size1(const Grid &g)
{
    return g.size1();
}
size_t size2(const Grid &g)
{
    return g.size2();
}
