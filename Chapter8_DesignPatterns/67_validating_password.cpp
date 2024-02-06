/**
 * @file 67_validating_password.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 67_validating_password.cpp
 * 
 * This file is solution to "Problem 67. Validating passwords"
 *  mentioned in "Chapter 8 : Design Patterns" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * 
 * This problem is solved using decorator design pattern. An abstract base class `AbstractPasswordCriteria`
 * is provided for interface enforcement for all derived classes. Any class that inherits
 * `AbstractPasswordCriteria` must implement the function `bool IsValid()`. Multiple classes such as 
 * - `LengthCriteria`
 * - `CaseCriteria`
 * - `SpecialCharacterCriteria`
 * - `DigitCriteria`
 * provide differet criteria for validating password.
 * At last `PasswordValidator` which combines all password validating techniques
 * for validating passwords/
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <string>
#include <locale>
#include <unordered_set>
#include <initializer_list>
#include <algorithm>
#include <memory>
#include <vector>
#include <iostream>
#include <functional>

using std::all_of;
using std::any_of;
using std::cbegin;
using std::cend;
using std::cout;
using std::initializer_list;
using std::islower;
using std::isupper;
using std::locale;
using std::shared_ptr;
using std::string;
using std::unordered_set;
using std::vector;

/**
 * @brief Abstract class representing the criteria for validating passwords.
 */
class AbstractPasswordCriteria
{
    public:
    virtual bool IsValid(const string &str) = 0;
    virtual ~AbstractPasswordCriteria() {}
};

/**
 * @brief Concrete class implementing password length criteria.
 * Lenght is specified as constructor argument.
*/
class LengthCriteria final : public AbstractPasswordCriteria
{
public:
    LengthCriteria(const size_t minimum_password_lenght) : minimum_length_{ minimum_password_lenght }
    {}

    virtual bool IsValid(const string &str)
    {
        return str.size() >= minimum_length_;
    }
private:
    const size_t minimum_length_;
};

/**
 * @brief Concrete class implementing password case criteria.
*/
class CaseCriteria final : public AbstractPasswordCriteria
{
public:
    CaseCriteria(const locale &loc = locale{}) : loc_{ loc }
    {

    }
    virtual bool IsValid(const string &str)
    {
        auto has_lower_case = false;
        auto has_upper_case = false;
        for (const auto &ch : str)
        {
            if (islower(ch, loc_)) { has_lower_case = true; }
            if (isupper(ch, loc_)) { has_upper_case = true; }
            if (has_lower_case && has_upper_case) { break; }
        }
        return has_lower_case && has_upper_case;
    }
private:
    locale loc_;
};

/**
 * @brief Concrete class implementing special character criteria.
 */
class SpecialCharacterCriteria final : public AbstractPasswordCriteria
{
public:
    SpecialCharacterCriteria(initializer_list<char> il) : character_set_{ il }
    {}
    virtual bool IsValid(const string &str)
    {
        return any_of(cbegin(str), cend(str), [this](const auto &ch){
            return character_set_.find(ch) != cend(character_set_);
        });
    }
private:
    unordered_set<char> character_set_;
};

/**
 * @brief Concrete class implementing digit criteria.
 */
class DigitCriteria final : public AbstractPasswordCriteria
{
public:
    virtual bool IsValid(const string &str)
    {
        return str.find_first_of("1234567890") != string::npos;
    }
};

/**
 * @brief Class for validating passwords based on multiple criteria.
 */
class PasswordValidator
{
public:
    PasswordValidator(initializer_list<shared_ptr<AbstractPasswordCriteria>> il) : criterias_list_{ il }
    {
    }
    bool Validate(const string &str)
    {
        return all_of(cbegin(criterias_list_), cend(criterias_list_), [&str](const auto &criteria){
            return criteria->IsValid(str);
        });
        // return all_of(cbegin(criterias_list_), cend(criterias_list_), std::bind(&AbstractPasswordCriteria::IsValid, std::placeholders::_1, str));
    }

private:
    vector<shared_ptr<AbstractPasswordCriteria>> criterias_list_;
};

int main()
{
    auto password_validator = PasswordValidator{{
        std::make_shared<LengthCriteria>(50),
        std::make_shared<CaseCriteria>(),
        std::make_shared<SpecialCharacterCriteria>(initializer_list{'~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')'}),
        std::make_shared<DigitCriteria>()
    }};
    cout << "Is password valid: " << password_validator.Validate("Usama@123456") << '\n';
    return 0;
}