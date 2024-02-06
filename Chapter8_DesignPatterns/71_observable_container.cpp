/**
 * @file 71_observable_container.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 71_observable_container.cpp
 * 
 * This file is solution to "Problem 71. Observable vector container"
 *  mentioned in "Chapter 8 : Design Patterns" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * - AbstractVectorObserver
 *      An abstract base class defining observer methods for various vector events:
 *      ElementsAssigned, ElementPushed, ElementPopped, and VectorDestroyed.
 * - BasicVectorObserver
 *      A concrete observer class implementing the observer methods with simple console output.
 * - ObservableVector
 *      A templated class that extends std::vector and includes mechanisms for notifying observers about vector events.
 *      Constructor overloads and methods similar to std::vector. The `NotifyObservers` method invokes a specified
 *      function on all registered observers. Observers can be added and removed using `AddObserver` and `RemoveObserver`
 *      methods. Pushing and popping elements trigger notifications to observers. The destructor triggers the
 *      `VectorDestroyed` event.
 * 
 * Driver code : 
 * - Creates an observable vector and a observer.
 * - Registers the observer into vector.
 * - Perform operations on vector.
 *  
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <initializer_list>
#include <vector>
#include <functional>
#include <algorithm>

using std::begin;
using std::bind;
using std::cout;
using std::end;
using std::endl;
using std::function;
using std::initializer_list;
using std::invoke;
using std::remove;
using std::vector;

/**
 * @brief Abstract base class for vector observers.
 * 
 * Defines virtual methods to observe vector events.
 */
template<class T>
struct AbstractVectorObserver
{
    virtual void ElementsAssigned() = 0;
    virtual void ElementPushed(const T &value) = 0;
    virtual void ElementPopped(const T &value) = 0;
    virtual void VectorDestroyed() = 0;
};

/**
 * @brief Concrete implementation of vector observer with console output.
 * 
 * Implements the observer interface and prints messages to the console.
 */
template<class T>
struct BasicVectorObserver : AbstractVectorObserver<T>
{

    virtual void ElementsAssigned() override
    {
        cout << "Elements Assigned observed in BasicVectorObserver::ElementsAssigned()" << endl;
    }
    virtual void ElementPushed(const T &value) override
    {
        cout << "value = " << value << ", push observed in BasicVectorObserver::ElementPushed()" << endl;
    }
    virtual void ElementPopped(const T &value) override
    {
        cout << "value = " << value << ", pop observed in BasicVectorObserver::ElementPoped()" << endl;
    }
    virtual void VectorDestroyed() override
    {
        cout << "Vector destruction observed in BasicVectorObserver::VectorDestroyed()" << endl;
    }
};

/**
 * @brief ObservableVector class template.
 * 
 * A vector class that supports observers to track changes.
 */
template <class T>
class ObservableVector
{
private:
    vector<T>                           vec_;            /*! std::vector for storing data*/
    vector<AbstractVectorObserver<T>*>  observers_vec{}; /*! vector for storing all observers*/
    using vec_type = decltype(vec_);

    /**
     * @brief Notifies all observers using the event @param func
     * 
     * @tparam ObserverFunction 
     * @param func 
     */
    template<class ObserverFunction>
    void NotifyObservers(ObserverFunction func)
    {
        for (auto observer : observers_vec)
        {
            if (observer)
            {
                invoke(func, observer);
            }
        }
    }
public:
    using value_type        = T;
    using size_type	        = typename vec_type::size_type;
    using difference_type   = typename vec_type::difference_type;
    using reference         = value_type&;
    using const_reference   = const value_type&;
    using pointer           = typename vec_type::pointer;	 
    using const_pointer     = typename vec_type::const_pointer;	 
    using iterator          = typename vec_type::iterator; 
    using const_iterator    = typename vec_type::const_iterator; 
    using reverse_iterator  = typename vec_type::reverse_iterator;
    using const_reverse_iterator = typename vec_type::const_reverse_iterator;

    ObservableVector() = default;
    ObservableVector(size_t sz, const T &value = T{}) : vec_(sz, value) {}
    template <class Iterator>
    ObservableVector(Iterator first, Iterator last) : vec_{ first, last } {}
    ObservableVector(initializer_list<T> il) : vec_{ il } {}
    ObservableVector(const ObservableVector &) = delete;
    ~ObservableVector()
    {
        NotifyObservers(&AbstractVectorObserver<T>::VectorDestroyed);
    }
    
    void Assign(const size_t &sz, const T &value)
    {
        vec_.assign(sz, value);
        NotifyObservers(&AbstractVectorObserver<T>::ElementsAssigned);
    }

    template <class Iterator> void Assign(Iterator first, Iterator last)
    {
        vec_.assign(first, last);
        NotifyObservers(&AbstractVectorObserver<T>::ElementsAssigned);
    }
    void Assign(initializer_list<T> il)
    {
        vec_.assign(il);
        NotifyObservers(&AbstractVectorObserver<T>::ElementsAssigned);
    }

    const_reference operator[](const size_t &idx) const
    {
        return vec_[idx];
    }
    const_reference at(const size_t &idx) const
    {
        return vec_.at(idx);
    }
    const_iterator begin() const  { return vec_.begin(); }
    const_iterator cbegin() const { return vec_.cbegin(); }

    const_iterator end() const  { return vec_.end(); }
    const_iterator cend() const { return vec_.cend(); }

    const_reverse_iterator rbegin() const  { return vec_.rbegin(); }
    const_reverse_iterator crbegin() const { return vec_.crbegin(); }

    const_reverse_iterator rend() const  { return vec_.rend(); }
    const_reverse_iterator crend() const { return vec_.crend(); }

    bool empty()         const { return vec_.empty();    }
    size_type size()     const { return vec_.size();     }
    size_type max_size() const { return vec_.max_size(); }

    void clear() { vec_.clear(); }
    /**
     * @brief Insert a new element at end of vector and calls ElementPushed
     * for all the registered observers.
     * 
     * @param value 
     */
    void push_back(const T &value)
    {
        vec_.push_back(value);
        for (auto observer : observers_vec)
        {
            if (observer)
            {
                observer->ElementPushed(value);
            }
        }
    }

    /**
     * @brief Removes the last element from vector and calls ElementPopped
     * for all members.
     */
    void pop_back()
    {
        const auto kBackElem = vec_.back();
        vec_.pop_back();
        for (auto observer : observers_vec)
        {
            if (observer)
            {
                observer->ElementPopped(kBackElem);
            }
        }
    }
    void AddObserver(AbstractVectorObserver<T> *observer)
    {
        observers_vec.push_back(observer);
    }
    void RemoveObserver(AbstractVectorObserver<T> *observer)
    {
        auto pos = remove(begin(observers_vec), end(observers_vec), observer);
        observers_vec.erase(pos, end(observers_vec));
    }
};


int main()
{
    auto obs_vec = ObservableVector<int>{ 1, 2, 3 };
    auto observer1 = BasicVectorObserver<int>{};
    obs_vec.AddObserver(&observer1);
    obs_vec.push_back(4);
    obs_vec.pop_back();
    obs_vec.Assign(10, -1);
    return 0;
}