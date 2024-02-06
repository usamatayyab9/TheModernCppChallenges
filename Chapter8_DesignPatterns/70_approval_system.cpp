/**
 * @file 70_approval_system.cpp
 * @author Usama Tayyab (usamtayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 70_approval_system.cpp
 * 
 * This file is solution to "Problem 70. Approval system"
 *  mentioned in "Chapter 8 : Design Patterns" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Solution:
 * 
 * - Expense Structure
 *      Represents an expense with an amount and a description.
 * - AbstractRole Class
 *      Abstract base class for different roles in the expense approval system.
 *      Declares a pure virtual function CanApprove to check if a role can approve a given expense.
 *      Provides a virtual destructor for polymorphic behavior.
 * - EmployeeRole
 *      Concrete class representing an employee with a basic role. Can approve expenses with amounts
 *      up to $1000.
 * - TeamManagerRole
 *      Concrete class representing a team manager role. Can approve expenses with amounts up to $10,000.
 * - DepartmentManagerRole
 *      Concrete class representing a department manager role. Can approve expenses with amounts up to $100,000.
 * - PresidentRole
 *      Concrete class representing a president role. Can approve expenses of any amount.
 * - Employee Class
 *      Represents an employee in the expense approval system. Has a name, a role (via a unique pointer to
 *      an AbstractRole), and a line manager (via a shared pointer to another Employee). Implements the
 *      ApproveExpense method to approve expenses based on the employee's role and escalate to the line
 *      manager if necessary.
 * 
 * Driver code:
 * 
 * Creates multiple employees with different roles. Initializes different expenses.
 * Then calls `Approve` function on one of the employee objects.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <string>
#include <memory>

using std::cout;
using std::endl;
using std::make_shared;
using std::shared_ptr;
using std::string;
using std::unique_ptr;

/**
 * @brief Structure representing an expense.
*/
struct Expense
{
    int     amount;
    string  description;
};

/**
 * @brief Abstract class representing a role in the expense approval system.
 */
class AbstractRole
{
public:
    virtual bool CanApprove(const Expense &expense) = 0;
    virtual ~AbstractRole() {}
};

/**
 * @brief Concrete class representing an employee who can approve only a bidget of 1000.
 */
class EmployeeRole : public AbstractRole
{
    public:
    bool CanApprove(const Expense &expense) override final
    {
        return expense.amount <= 1000;
    }
};

/**
 * @brief Concrete class representing an manager..
 */
class TeamManagerRole : public AbstractRole
{
    public:
    bool CanApprove(const Expense &expense) override final
    {
        return expense.amount <= 10'000;
    }
};

/**
 * @brief Concrete class representing a department manager.
 */
class DepartmentManagerRole : public AbstractRole
{
    public:
    bool CanApprove(const Expense &expense) override final
    {
        return expense.amount <= 1'00'000;
    }
};

/**
 * @brief Concrete class representing a president.
 */
class PresidentRole : public AbstractRole
{
    public:
    bool CanApprove(const Expense &expense) override final
    {
        return true;
    }
};

class Employee
{
    string                   name_{};         /**< The name of the employee. */
    unique_ptr<AbstractRole> role_;           /**< The role of the employee. */
    shared_ptr<Employee>     line_manager_{}; /**< The line manager of the employee. */
public:
    Employee() = delete;

    /**
     * @brief Constructor for creating an employee with a specified name, role, and line manager.
     *
     * @param name The name of the employee.
     * @param role The role of the employee.
     * @param manager The line manager of the employee.
     */
    Employee(const string &name, unique_ptr<AbstractRole> role, shared_ptr<Employee> manager = nullptr):
        name_{ name }, role_{ std::move(role) }, line_manager_{ manager }
    {}

    /**
     * @brief Approve an expense based on the employee's role and escalate if needed.
     *
     * @param expense The expense to be approved.
     */
    void ApproveExpense(const Expense &expense)
    {
        if (role_->CanApprove(expense))
        {
            cout << "Expense for " << expense.description << " approved by " << name_ << endl;
        }
        else if (line_manager_)
        {
            cout << name_ << " cant't approve the expense " << expense.description << " amounting " << expense.amount << endl;
            cout << "Sending to " << line_manager_->name_ << " for approval" << endl;
            line_manager_->ApproveExpense(expense);
        }
    }
};

int main()
{
    auto president    = make_shared<Employee>( "Mr. President", std::make_unique<PresidentRole>() );
    auto dept_manager = make_shared<Employee>( "Mr. Manager", std::make_unique<DepartmentManagerRole>(), president );
    auto team_lead    = make_shared<Employee>( "Mr. Lead", std::make_unique<TeamManagerRole>(), dept_manager );
    auto employee     = make_shared<Employee>( "Mr. Employee", std::make_unique<EmployeeRole>(), team_lead );

    auto expense1 = Expense{ 1000, "Lunch"};
    auto expense2 = Expense{ 5000, "Trip budget"};
    auto expense3 = Expense{ 10'000, "Furniture"};
    auto expense4 = Expense{ 1'00'000, "Office renovation"};
    auto expense5 = Expense{ 5'00'000, "New Office"};
    
    employee->ApproveExpense(expense1); cout << endl;
    employee->ApproveExpense(expense2); cout << endl;
    employee->ApproveExpense(expense3); cout << endl;
    employee->ApproveExpense(expense4); cout << endl;
    employee->ApproveExpense(expense5); cout << endl;
    
    return 0;
}