/**
 * @file 68_generating_random_passwords.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 68_generating_random_passwords.cpp
 * 
 * This file is solution to "Problem 68. Generating random passwords"
 *  mentioned in "Chapter 8 : Design Patterns" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Driver code :
 * - Initializes a password generator with requirements.
 * - Input from user on how many password to generate.
 * - Generates passwords.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include <iostream>
#include <random>
#include <numeric>
#include <iterator>

using std::accumulate;
using std::cbegin;
using std::cend;
using std::cin;
using std::cout;
using std::mt19937;
using std::ostream_iterator;
using std::random_device;
using std::shared_ptr;
using std::shuffle;
using std::string;
using std::uniform_int_distribution;
using std::vector;

/**
 * @brief Abstract class representing basic password requirements.
 */
class BasicPasswordRequirement
{
    public:
    /**
     * @brief Constructor for BasicPasswordRequirement.
     *
     * @param len The required length for the password.
     */
    BasicPasswordRequirement(const size_t &len) : password_lenght_{ len }
    {}
    virtual string AllowedCharacters() const = 0;
    size_t length() const { return password_lenght_; }
    private:
    size_t password_lenght_{ 0 };
};

/**
 * @brief Concrete class implementing digit password requirement.
 */
class DigitRequirement : public BasicPasswordRequirement
{
    public:
    DigitRequirement(const size_t &len) : BasicPasswordRequirement(len)
    {}
    string AllowedCharacters() const override 
    {
        return "0123456789";
    }
};

/**
 * @brief Concrete class implementing uppercase letter password requirement.
 */
class UpperCaseRequirement : public BasicPasswordRequirement
{
    public:
    UpperCaseRequirement(const size_t &len) : BasicPasswordRequirement(len)
    {}
    string AllowedCharacters() const override 
    {
        return "ABCDEFGHIJKLMNOPQRSTUVXYWZ";
    }
};

/**
 * @brief Concrete class implementing lowercase letter password requirement.
 */
class LowerCaseRequirement : public BasicPasswordRequirement
{
    public:
    LowerCaseRequirement(const size_t &len) : BasicPasswordRequirement(len)
    {}
    string AllowedCharacters() const override 
    {
        return "abcdefghijklmnopqrstuvxywz";
    }
};

/**
 * @brief Concrete class implementing special characters password requirement.
 */
class SpecialCharacterRequirement : public BasicPasswordRequirement
{
    public:
    SpecialCharacterRequirement(const size_t &len) : BasicPasswordRequirement(len)
    {}
    string AllowedCharacters() const override 
    {
        return "`~!@#$%^&*()-=_+[]{}|;':,./<>?";
    }
};

/**
 * @brief Class for generating passwords based on specified requirements.
 */
class PasswordGenerator
{
public:
    PasswordGenerator() = default;
    /**
     * @brief Add a password requirement to the generator.
     *
     * @param requirement A shared pointer to a BasicPasswordRequirement.
     */
    void AddRequirement(shared_ptr<BasicPasswordRequirement> requiremnt)
    {
        requirements_.push_back(requiremnt);
    }

    /**
     * @brief Generate a password based on the specified requirements.
     *
     * @return The generated password.
     */
    string Generate()
    {
        /*! Pre compute password length to avoid reallocations*/
        const auto kPasswordLength = accumulate(cbegin(requirements_), cend(requirements_), 0,
        [](const auto &init, const auto &requirement)
        {
            return init + requirement->length();
        });
        /*! Add characters sequentially from all reuirments*/
        auto password = string( kPasswordLength, '\0' );
        auto iter     = begin(password);
        for (const auto &requirement : requirements_)
        {
            const auto kAllowedCharacters = requirement->AllowedCharacters();
            const auto kLength            = requirement->length();
            auto distribution             = uniform_int_distribution<>( 0, kLength - 1 );
            for (auto idx = size_t{ 0 }; idx < kLength ;++idx)
            {
                *iter = kAllowedCharacters[distribution(gen_)];
                ++iter;
            }
        }

        /*! Same as above. Only uses accumulate.*/
        // accumulate(cbegin(requirements_), cend(requirements_), begin(password),
        // [this](auto iter, const auto &requirement){
        //     const auto kAllowedCharacters = requirement->AllowedCharacters();
        //     const auto kLength            = requirement->length();
        //     auto distribution = uniform_int_distribution<>( 0, kLength - 1 );
        //     for (auto idx = size_t{ 0 }; idx < kLength ;++idx)
        //     {
        //         *iter = kAllowedCharacters[distribution(gen_)];
        //         ++iter;
        //     }
        //     return iter;
        // });

        /*! Once all all reuqirments are met shuffle the string*/
        shuffle(begin(password), end(password), gen_);
        
        return password;
    }

    string operator()()
    {
        return Generate();
    }
private:
    random_device              rd_{};
    mt19937                    gen_{ rd_() };
    vector<shared_ptr<BasicPasswordRequirement>> requirements_{};
};

int main()
{
    auto password_generator = PasswordGenerator{};
    password_generator.AddRequirement(std::make_shared<DigitRequirement>(4));
    password_generator.AddRequirement(std::make_shared<LowerCaseRequirement>(4));
    password_generator.AddRequirement(std::make_shared<UpperCaseRequirement>(4));
    password_generator.AddRequirement(std::make_shared<SpecialCharacterRequirement>(4));
    
    auto total = 0;
    cout << "How many random passwords you want to generate? ";
    cin >> total;
    std::generate_n(ostream_iterator<string>{ cout, "\n" }, total, std::ref(password_generator));

    return 0;
}