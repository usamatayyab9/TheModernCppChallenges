/**
 * @file 48_most_frequent_element.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 48_most_frequent_element.cpp
 * 
 *  This file is solution to "Problem 48. The most frequent element in a range"
 *  mentioned in "Chapter 6: Algorithms and Data Structures" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a function that, given a range, returns the most frequent element
 * and the number of times it appears in the range. If more than one
 * element appears the same maximum number of times then the function
 * should return all the elements. For instance, for the range
 * {1,1,3,5,8,13,3,5,8,8,5}, it should return {5, 3} and {8, 3}.
 * 
 * Solution:
 * The struct `MostFrequentElementCalculator` implements the solution.
 * The struct overload the function operator which takes a templated
 * container as input and returns most frequent elements along with
 * their frequency. The results are returned in as an object of
 * type `FrequencyCalculatorResults` which contains two members:
 *  - frequency : frequency of most frequent element.
 *  - vector    : All the elements that occur most frequently.
 * 
 * Driver code:
 * - Initializes the function object for calculating most
 *      frequent elements.
 * - Passes a vector to above functor.
 * - Prints all the elements that are most frequent.
 * 
 * 
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <vector>
#include <utility>
#include <type_traits>
#include <unordered_map>
#include <algorithm>

using std::cout;
using std::cbegin;
using std::cend;
using std::endl;
using std::max_element;
using std::pair;
using std::vector;
using std::unordered_map;

template<class T>
struct FrequencyCalculatorResults
{
    size_t      frequency;
    vector<T>   elements;
};

struct MostFrequentElementCalculator
{
    template<class Container>
    auto ComputeElementFrequency(const Container &C)
    {
        using KeyType = typename Container::value_type;

        auto frequency_umap = unordered_map<KeyType, size_t>{};
        for (const auto &elem : C) { ++frequency_umap[elem]; }

        return frequency_umap;
    }

    template<class Container>
    auto operator()(const Container &C)
    {
        using ValueType = typename Container::value_type;
        /*! Step 1 Frequency map. For each distinct element in param C stores its frequency*/
        const auto kFrequencyUmap   = ComputeElementFrequency(C);
        /*! Step 2 Calculate highest frequency from step 1 generated map */
        const auto kMaxFrequentElem = max_element(cbegin(kFrequencyUmap), cend(kFrequencyUmap),
        [](const auto &elem1, const auto &elem2){
            return elem1.second < elem2.second;
        });
        const auto kMaxFrequency = kMaxFrequentElem->second;
        /*! Step 3 Traverse the map from step 1 for each frequency that matches the highest frequency
            from step 2 store its corresponding value in vector;
        */
        auto results = FrequencyCalculatorResults<ValueType>{};
        results.frequency = kMaxFrequency;

        for (const auto &[kElem, kFrequency] : kFrequencyUmap)
        {
            if (kFrequency == kMaxFrequency)
            {
                results.elements.push_back(kElem);
            }
        }

        return results;
    }
};

int main()
{
    auto frequent_element_finder = MostFrequentElementCalculator{};
    const auto [kFrequency, kElements] = frequent_element_finder(vector{ 1, 1, 2, 3, 4, 5, 1, 1, 1, 2, 2, 2, 2 });
    for (const auto &kElem : kElements)
    {
        cout << kElem << ':' << kFrequency << '\n';
    }
    return 0;
}