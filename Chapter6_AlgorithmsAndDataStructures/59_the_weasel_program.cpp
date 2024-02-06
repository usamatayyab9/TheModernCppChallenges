/**
 * @file 59_the_weasel_program.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 59_the_weasel_program.cpp
 *  This file is solution to "Problem 59. The Weasel program"
 *  mentioned in "Chapter 6: Algorithms and Data Structures" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *
 * Solution:
 * This solution requires implementing dawkin weasel program. The simulation
 * is provided by struct `DawkinWeaselSimulator` which takes a fitness function
 * as template argument. The function operator is overloaded by takes a target
 * string and generates a initial random string keeps mutating it until target
 * string is achieved.
 * 
 * Driver code:
 * Initalizes the `DawkinWeaselSimulator` object with `InverseHammingDistance` as 
 * fitness function. Then calls function operator.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <string>
#include <algorithm>
#include <cassert>
#include <numeric>
#include <functional>
#include <utility>
#include <vector>
#include <tuple>
#include <thread>
#include <chrono>
#include <random>

using std::cbegin;
using std::cend;
using std::cout;
using std::default_random_engine;
using std::endl;
using std::equal_to;
using std::generate;
using std::inner_product;
using std::max_element;
using std::pair;
using std::plus;
using std::size;
using std::string;
using std::string_view;
using std::this_thread::sleep_for;
using std::tuple;
using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::vector;

using namespace std::chrono_literals;

/**
 * @brief A structure for calculating inverse hamming distance between two strings.
 * The Hamming distance between two equal-length strings of symbols is the number
 * of positions at which the corresponding symbols are different.
 * 
 * Hence inverse is calculated as number of positions at which corresponding
 * characters are same.
 * 
 */
struct InverseHammingDistance
{
    int operator()(const string &str1, const string &str2)
    {
        assert(size(str1) == size(str2));
        return inner_product(cbegin(str1), cend(str1), cbegin(str2), 0, plus{}, equal_to{});
    }
};

template <class FitnessFunction>
class DawkinWeaselSimulator
{
public:
    DawkinWeaselSimulator()
    {
        auto device = std::random_device{};
        engine_.seed(device());
    }
    DawkinWeaselSimulator(size_t copies_arg, float mutation) : DawkinWeaselSimulator()
    {
        copies_          = copies_arg;
        mutation_chance_ = mutation;
    }

    /**
     * @brief 
     *          Simulate weasel program according to the below criter
     *          - if @param iterations <= -1 then simulation executes indefinitely
     *            or until target string is generated.
     *          - if @param iterations > -1 then simulation is executed for `iterations`
     *            times or until target string is generated, whichever happens first.
     * @param target 
     * @param iterations 
     */
    void operator()(const string &target, const int &iterations)
    {
        for (auto [parent, offsprings, i] = tuple{ GetRandomString(size(target)), vector<string>(copies_), 0, }; 
            true;
            ++i)
        {
            cout << "Iteration: " << i << " " << parent << "\n"; 
            if (const auto kContinueLoop = (iterations <= -1 || i < iterations) && target != parent;
                false == kContinueLoop)
            {
                break;
            }
            
            for (auto &child : offsprings)
            {
                child = GetMutationString(parent);
            }

            parent = *max_element(cbegin(offsprings), cend(offsprings), [&target, this](const auto &largest, const auto &current){
                return fitness_function_(current, target) > fitness_function_(largest, target);
            });

            sleep_for(10ms);
        }
    }

    private:
    string GetRandomString(const size_t &n)
    {
        auto str = string(n, '\0');
        for (auto &ch : str)
        {
            ch = kCharacterSet[character_distribution_(engine_)];
        }
        return str;
    }
    string GetMutationString(const string &parent)
    {
        auto mutated_string = parent;
        for (auto &ch : mutated_string)
        {
            if (mutation_rate_distribution_(engine_) <= mutation_chance_)
            {
                ch = kCharacterSet[character_distribution_(engine_)];
            }
        }
        return mutated_string;
    }

    static constexpr string_view kCharacterSet{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ " };
    size_t                           copies_                    { 100 }; // Number of copies (offsprings) produced in each iteration
    float                            mutation_chance_           { 5.0f };
    FitnessFunction                  fitness_function_          {   };
    uniform_int_distribution<>       character_distribution_    { 0, kCharacterSet.size() - 1 };
    uniform_real_distribution<float> mutation_rate_distribution_{ 0.0f, 100.0f };
    default_random_engine            engine_;
};

int main()
{
    auto weasel_program_simulator = DawkinWeaselSimulator<InverseHammingDistance>{};
    weasel_program_simulator("METHINKS IT IS LIKE A WEASEL", -1);
    return 0;
}