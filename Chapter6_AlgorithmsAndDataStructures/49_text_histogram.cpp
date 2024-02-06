/**
 * @file 49_text_histogram.cpp
 * @author Usama Tayyabb (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 49_text_histogram.cpp
 * 
 *  This file is solution to "Problem 49. Text histogram"
 *  mentioned in "Chapter 6: Algorithms and Data Structures" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *  
 * Problem statement:
 * Write a program that, given a text, determines and prints a
 * histogram with the frequency of each letter of the alphabet.
 * The frequency is the percentage of the number of appearances
 * of each letter from the total count of letters. The program
 * should count only the appearances of letters and ignore digits,
 * signs, and other possible characters. The frequency must be
 * determined based on the count of letters and not the text size
 * 
 * Solution:
 * The function `ComputeTextHistogram` takes a string as input
 * and returns the text histogram. The function firsts computes 
 * frequencies of all alphabets and then calculates histogram
 * values for all alphabets.
 * 
 * Driver code is same as author's. 
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <algorithm>
#include <iterator>

using std::cbegin;
using std::cend;
using std::cout;
using std::endl;
using std::string;
using std::string_view;
using std::transform;
using std::unordered_map;

auto ComputeTextHistogram(string_view text)
{
    auto char_frequency_umap = unordered_map<char, int>{};
    for (auto ch = 'a'; ch <= 'z' ;++ch) { char_frequency_umap[ch] = 0; }

    auto total_alphabets = 0;
    for (const auto &ch : text)
    {
        if (isalpha(ch)) {
            ++char_frequency_umap[tolower(ch)];
            ++total_alphabets;
        }
    }

    auto histogram_umap = unordered_map<char, float>{};
    for (const auto &[letter, frequency] : char_frequency_umap)
    {
        histogram_umap.insert({ letter, 100.0f * frequency / static_cast<float>(total_alphabets) });
    }
    return histogram_umap;
}

int main()
{
    /*! Driver code for main taken from problem's solution section*/
    constexpr auto text = string_view("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
    
    const auto kHistogram = ComputeTextHistogram(text);
    for (auto const &[letter, ocurring_percentage] : kHistogram)
    {
        cout << letter << " : " << ocurring_percentage << '\n';
    }
    return 0;
}