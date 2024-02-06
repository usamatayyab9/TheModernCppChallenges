/**
 * @file 17_2d_array.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 *      Compilation command: g++ -std=c++17 17_2d_array.cpp
 * This file is solution to "Problem 16. Creating a 2D array with basic operations"
 * mentioned in "Chapter 2: Language Features" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 *  "Write a class template that represents a two-dimensional array container with methods for
 *   element access (at() and data()), capacity querying, iterators, filling, and swapping. It
 *   should be possible to move objects of this type."
 * 
 * This file implemnts the class `Array2D` for implementing all feature mentioned in the problem
 * Driver code:
 * - creates a two dimensional array of dimention 2x3 and initializes it with values 1,2,3,4,5,6.
 * - All elements of above array are printed on console indicating the use of index access provided
 *      for reading a specific row, column.
 * - All elements of above array are multiplied by using nested for loops and function operator which
 *      is used for updating value of a particular index too.
 * - All elements of above array then printed on console using iterators.
 * - All elements are assigned the same value using Fill() function provided by the class.
 * - All elements of above array then printed on console using iterators hence verifying that
 *      Fill() function fills correctly.
 * - Another two dimensional array is created but this time a constepxr specified is used indicating
 *      that compile-time array initialization can also be done.
 * - Some values on constexpr array are fetched using row, column indices denoting that constexpr
 *      values can also be access at compile time.
 *      
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <array>
#include <initializer_list>
#include <algorithm>
#include <iterator>

using std::array;
using std::cout;
using std::copy;
using std::begin;
using std::cbegin;
using std::end;
using std::endl;
using std::cend;
using std::initializer_list;
using std::ostream_iterator;

template <class T, size_t Rows, size_t Columns>
class Array2D
{
public:
    using value_type        = T;
    using size_type         = std::size_t;
    using difference_type   = std::ptrdiff_t;
    using reference	        = value_type&;
    using const_reference   = const value_type&;
    using pointer           = value_type*;
    using const_pointer     = const value_type*;
    using iterator          = value_type*;
    using const_iterator    = const value_type*;
    
    constexpr Array2D()
    {
        arr.fill(value_type{});
    }
    template <typename... Args>
    constexpr Array2D(Args... arguments) : arr{ arguments... }
    {

    }

    constexpr value_type& At(const size_t &r_idx, const size_t &c_idx)
    {
        return arr.at(r_idx * Columns + c_idx);
    }
    constexpr const value_type& At(const size_t &r_idx, const size_t &c_idx) const
    {
        return arr.at(r_idx * Columns + c_idx);
    }
    constexpr value_type& operator()(const size_t &r_idx, const size_t &c_idx)
    {
        return arr[r_idx * Columns + c_idx];
    }
    constexpr const value_type& operator()(const size_t &r_idx, const size_t &c_idx) const
    {
        return arr[r_idx * Columns + c_idx];
    }
    
    constexpr pointer       Data()       { return arr.data(); }
    constexpr const_pointer Data() const { return arr.data(); }
    
    constexpr void Fill(const value_type &arg)
    {
        arr.fill(arg);
    }

    void Swap(Array2D<T, Rows, Columns> &arg)
    {
        if (this != &arg)
        {
            arr.swap(arg.arr);
        }
    }

    constexpr size_t RowCount()    const { return Rows;    }
    constexpr size_t ColumnCount() const { return Columns; }
    
    constexpr iterator       begin()        { return arr.data();                  }
    constexpr const_iterator begin()  const { return arr.data();                  }
    constexpr const_iterator cbegin() const { return arr.data();                  }
    
    constexpr iterator       end()        { return arr.data() + std::size(arr); }
    constexpr const_iterator end()  const { return arr.data() + std::size(arr); }
    constexpr const_iterator cend() const { return arr.data() + std::size(arr); }

private:
    array<T, Rows * Columns> arr;
};

int main()
{
    auto arr = Array2D<int, 2, 3>{ 1, 2, 3, 4, 5, 6 };
    for (auto r = 0; r < arr.RowCount(); ++r) {
        for (auto c = 0; c < arr.ColumnCount(); ++c) {
            cout << arr(r, c) << " ";
        }
        cout << endl;
    }

    for (auto r = 0; r < arr.RowCount(); ++r) {
        for (auto c = 0; c < arr.ColumnCount(); ++c) {
            arr(r, c) *= 2;
        }
    }
    copy(arr.cbegin(), arr.cend(), ostream_iterator<decltype(arr)::value_type>{ cout, "\n" });
    
    arr.Fill(0);
    copy(arr.cbegin(), arr.cend(), ostream_iterator<decltype(arr)::value_type>{ cout, "\n" });

    constexpr auto kArr2 = Array2D<int, 2, 2>{ 1, 2, 3, 4 };
    constexpr auto kVal1 = kArr2(0, 0);
    constexpr auto kVal2 = kArr2.At(0, 1);

    constexpr auto kSum = kVal1 + kVal2;
    cout << "Sum: " << kSum << endl;
    
    return 0;
}