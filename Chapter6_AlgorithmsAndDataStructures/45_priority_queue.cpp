/**
 * @file 45_priority_queue.cpp
 * @author Usama Tayyab (usamatayya9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 45_priority_queue.cpp
 * 
 *  This file is solution to "Problem 45. Priority queue"
 *  mentioned in "Chapter 6: Algorithms and Data Structures" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 *  Write a data structure that represents a priority queue that provides constant time lookup
 *  for the largest element, but has logarithmic time complexity for adding and removing
 *  elements. A queue inserts new elements at the end and removes elements from the top. By
 *  default, the queue should use operator< to compare elements, but it should be possible
 *  for the user to provide a comparison function object that returns true if the first argument
 *  is less than the second. The implementation must provide at least the following operations:
 *      - push() to add a new element
 *      - pop() to remove the top element
 *      - top() to provide access to the top element
 *      - size() to indicate the number of elements in the queue
 *      - empty() to indicate whether the queue is empty
 * 
 * Solution:
 * Binary heap is implemented using std::vector. Element type of priority is specified via
 * templates parameter also the ordering of of elements is specified throught template
 * parameter.
 * 
 * Driver code:
 * Program demostrates the use of priority queue by creating it, inserting elements,
 * querying its size, removing element from it.
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <iostream>
#include <vector>
#include <functional>

using std::cout;
using std::endl;
using std::vector;
using std::size;
using std::swap;

/**
 * @brief A binary heap implementation
 * 
 * @tparam T - Element type 
 * @tparam Comparator - Ordering criteria for elements
 */
template <class T, class Comparator>
class PriorityQueue
{
public:
    using Container       = vector<T>;
    using value_comprator = Comparator;
    using value_type      = typename Container::value_type;
    using size_type       = typename Container::size_type;
    using const_reference = typename Container::const_reference;

    PriorityQueue() = default;
    
    void Push(const value_type &value)
    {
        heap_data_.push_back(value);
        /*! No need to heapify for one element*/
        if (1 != size(heap_data_))
        {
            HeapifyUp(size(heap_data_) - 1);
        }
    }

    void Pop()
    {
        if (0 != Size())
        {
            swap(heap_data_.front(), heap_data_.back());
            heap_data_.pop_back();
            HeapifyDown(0);
        }
    }

    const_reference Top() { return heap_data_[0]; }
    size_type Size() const { return size(heap_data_); }
    bool Empty() const { return 0 == Size(); }

private:
    size_t GetParent(const size_t &idx) const
    {
        if (0 == (idx % 2)) { return (idx / 2) - 1; }
        return idx / 2;
    }
    size_t GetLeft(const size_t &idx) const
    {
        return 2 * idx + 1;
    }
    size_t GetRight(const size_t &idx) const
    {
        return 2 * idx + 2;
    }

    void HeapifyUp(const size_t &idx)
    {
        if (0 != idx)
        {
            auto parent_pos = GetParent(idx);
            if (!comp_(heap_data_[parent_pos], heap_data_[idx]))
            {
                swap(heap_data_[parent_pos], heap_data_[idx]);
                HeapifyUp(parent_pos);
            }
        }
    }

    void HeapifyDown(const size_t &idx)
    {
        auto pos = idx;
        if (const auto kLeftIdx = GetLeft(idx); kLeftIdx < size(heap_data_)
            && comp_(heap_data_[kLeftIdx], heap_data_[pos]))
        {
            pos = kLeftIdx;
        }
        if (const auto kRightIdx = GetRight(idx); kRightIdx < size(heap_data_)
            && comp_(heap_data_[kRightIdx], heap_data_[pos]))
        {
            pos = kRightIdx;
        }

        if (idx != pos)
        {
            swap(heap_data_[idx], heap_data_[pos]);
            HeapifyDown(pos);
        }
    }

    Container heap_data_;
    Comparator comp_;
};

int main()
{
    auto pq = PriorityQueue<int, std::greater<int>>{};
    for (const auto i : { 2, 9, 4, 1, 7, 6, 3, 5, 10, 8 })
    {
        pq.Push(i);
    }
    cout << "Size = " << pq.Size() << endl;
    while (false == pq.Empty())
    {
        cout << pq.Top() << endl;
        pq.Pop();
    }
    return 0;
}