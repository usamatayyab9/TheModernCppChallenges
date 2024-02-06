/**
 * @file 45_priority_queue.cpp
 * @author Usama Tayyab (usamatayya9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 46_circular_buffer.cpp
 * 
 *  This file is solution to "Problem 46. Circular buffer"
 *  mentioned in "Chapter 6: Algorithms and Data Structures" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Create a data structure that represents a circular buffer of a fixed size.
 * A circular buffer overwrites existing elements when the buffer is being filled
 * beyond its fixed size. The class you must write should:
 *  - Prohibit default construction
 *  - Support the creation of objects with a specified size
 *  - Allow checking of the buffer capacity and status(empty(), full(), size(),
 *      capacity())
 *  - Add a new element, an operation that could potentially overwrite the oldest
 *      element in the buffer
 *  - Remove the oldest element from the buffer
 *  - Support iteration through its elements
 * 
 * Solution:
 * The class `CircularBuffer` implements all the required functionality.
 * Along with iterating over the elements.
 * 
 * Driver code:
 * - Creates circular buffer of size 5.
 * - Insert elements into it.
 * - Print buffer state
 * - Uses different STL algorithms with circual buffer.
 *
 * @copyright Copyright (c) 2023 
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <cassert>

using std::cout;
using std::endl;
using std::vector;

/**
 * @brief
 * 
 * @tparam T 
 */
template <class T>
class CircularBuffer
{
public:
    using value_type = T;
    using size_type  = size_t;
    
    CircularBuffer() = delete;

    CircularBuffer(const size_t &n) : vec_(n) {}

    bool Empty() const { return 0 == size_; }
    
    bool Full() const { return size_ == vec_.size(); }
    
    size_type Size() const { return size_; }
    
    size_type Capacity() const { return vec_.size(); }

    void Push(const value_type &value)
    {
        vec_[insert_pos_] = value;
        ++insert_pos_;
        if (insert_pos_ == Capacity()) { insert_pos_ = 0; }
        if (Full()) { head_ = insert_pos_; }
        if (size_ < vec_.size()) { ++size_; }
    }

    void Pop()
    {
        if (0 == Size()) { return; }
        --size_;
        ++head_;
        if (head_ == vec_.size()) { head_ = 0; }
    }

    void Print()
    {
        cout << "insert_pos_:" << insert_pos_ << endl;
        cout << "head_       :" << head_ << endl;
        cout << "size_      :" << size_ << endl;
        cout << "[";

        for (auto idx = size_type{ 0 }; idx < Size() ;++idx)
        {
            cout << vec_[(idx + head_) % vec_.size()] << ", ";
        }
        cout << "]\n";
    }

    /**
     * @brief A class for iterating over the elements of circular buffer.
     * A object is constructed with a reference to CircularBuffer object and
     * its startting position
     * 
     */
    class CircularBufferIterator
    {
        public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using reference         = T&;
        using const_reference   = const value_type &;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;

        CircularBufferIterator(CircularBuffer &buffer, size_t pos) :
            buffer_ref{ buffer },
            idx_ { pos }
        {
        }

        //pre-increment
        CircularBufferIterator& operator++() {
            ++idx_;
            return *this;
        }

        //post-increment
        CircularBufferIterator operator++(int) {
            auto old = *this;
            ++idx_;
            return old;
        }

        reference operator*()
        {
            return buffer_ref.vec_[(idx_ + buffer_ref.head_) % buffer_ref.Capacity()];
        }
        const_reference operator*() const
        {
            return buffer_ref.vec_[(idx_ + buffer_ref.head_) % buffer_ref.Capacity()];
        }

        bool operator==(const CircularBufferIterator &rhs) const {
            assert(IsIteratorToSameBuffer(rhs));
            return idx_ ==  rhs.idx_; 
        }

        bool operator!=(const CircularBufferIterator &rhs) const {
            return !(*this == rhs);
        }

        bool IsValid() const
        {
            return idx_ < buffer_ref.Size();
        }

        private:
        bool IsIteratorToSameBuffer(const CircularBufferIterator &other_buffer_iterator) const
        {
            return std::addressof(buffer_ref) == std::addressof(other_buffer_iterator.buffer_ref);
        }
        CircularBuffer &buffer_ref;
        size_t idx_{ 0 };
    };

    CircularBufferIterator begin() { return CircularBufferIterator{ *this, 0 }; }
    CircularBufferIterator end() { return CircularBufferIterator{ *this, Size() }; }
    private:
    vector<value_type> vec_; //storage for elements
    size_type          insert_pos_{ 0 }; // Position where new element will be inserted
    size_type          head_{ 0 };      // Position of the oldest element
    size_type          size_{ 0 };      // Number of element in buffer

};

int main()
{
    auto buffer = CircularBuffer<int>(5);
    buffer.Print();

    for (const auto i : {600, 700, 800, 900, 1000, 1500, 2500, 3500, 4000})
    {
        buffer.Push(i);
        // buffer.Print();
        // cout << "\n\n";
    }

    buffer.Print();

    std::transform(buffer.begin(), buffer.end(), buffer.begin(), [](const auto &val){
        return val + 1;
    });

    std::for_each_n(buffer.begin(), buffer.Size(), [](auto &val){
        cout << val << endl;
    });
    cout << endl;
    
    std::transform(buffer.begin(), buffer.end(), buffer.begin(), [](const auto &val){
        return val * 2;
    });
    for (auto first = buffer.begin(), last = buffer.end(); first != last; ++first)
    {
        cout << *first << endl;
    }
    cout << endl;

    std::copy(buffer.begin(), buffer.end(), std::ostream_iterator<int>{ cout, " " });

    for (const auto &val : buffer)
    {
        cout << val << ',';
    }

    return 0;
}