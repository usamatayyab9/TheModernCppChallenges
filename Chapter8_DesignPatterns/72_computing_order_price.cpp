/**
 * @file 72_computing_order_price.cpp
 * @author Usama Tayyab (usamtayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 72_computing_order_price.cpp
 * This file is solution to "Problem 72. Computing order price with discounts"
 *  mentioned in "Chapter 8 : Design Patterns" of the book:
 *  - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 * 
 * Driver code:
 * - Creates different types of discounts.
 * - Creates different articles
 * - Creates different customers each with a order.
 * - Calcultes the final bill for each customer.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

/**
 * @brief Abstract base class for discount strategies.
 * 
 * Defines a virtual method to calculate the discount percentage.
 */
struct Discount
{
    virtual double DiscountPercentage(const double &, const double &) const noexcept = 0;
    virtual ~Discount() {}
    protected:

    protected:
    double discount_percentage;
};

struct FixedDiscount : public Discount
{
    FixedDiscount(const double &value) 
    {
        discount_percentage = value;
    }
    double DiscountPercentage(const double &, const double &) const noexcept override
    {
        return discount_percentage;
    }
};

struct VolumeDiscount : public Discount
{
    VolumeDiscount(const int &quantity, const double &discount)
    {
        discount_percentage = discount;
        min_quantity        = quantity;
    }
    double DiscountPercentage(const double &quantity_ordered, const double &) const noexcept override
    {
        return quantity_ordered >= min_quantity ? discount_percentage : 0;
    }
    private:
    int min_quantity;    
};

struct PriceDiscount : public Discount
{
    PriceDiscount(const int &price, const double &discount)
    {
        discount_percentage = discount;
        min_price           = price;
    }
    double DiscountPercentage(const double &quantity, const double &price) const noexcept override
    {
        return (price * quantity >= min_price) ? discount_percentage : 0;
    }
    private:
    double min_price;
};

/**
 * @brief Concrete class representing an amount-based discount strategy.
 * 
 * Provides a discount based on the total amount meeting a minimum amount threshold.
 */
struct AmountDiscount : public Discount
{
    AmountDiscount(const double &price, const double &discount)
    {
        discount_percentage = discount;
        min_price           = price;
    }
    double DiscountPercentage(const double &, const double &total_amount) const noexcept override
    {
        return total_amount >= min_price ? discount_percentage : 0;
    }
    private:
    double min_price;
};

/**
 * @brief Struct representing an article with an associated discount strategy.
 */
struct Article
{
    int                 id;
    string              name;
    double              price;
    const Discount*     discount;
};

struct OrderItem
{
    Article             article;
    int                 quantity;
    const Discount*     discount;
};

struct CustomerOrder
{
    vector<OrderItem>   order_items;
    const Discount*     discount;
};

struct Customer
{
    int             id;
    string          name;
    CustomerOrder   order;
    const Discount* discount;
};

/**
 * @brief Calculates a percentage of one value from another.
 * 
 * @param x The percentage value.
 * @param y The total value.
 * @return double The result of x percent of y.
 */
double XPercentOfY(const double &x, const double &y)
{

    return y * (x / 100.0);
}

/**
 * @brief Calculates the total price for a customer's order considering all discounts.
 * 
 * @param customer The customer for whom to calculate the total price.
 * @return double The total price after applying all discounts.
 */
double CaclulateTotalPriceFor(const Customer &customer)
{
    auto total_price = double{ 0 };
    for (const auto &kOrderItem : customer.order.order_items)
    {
        const auto kUnitPrice        = kOrderItem.article.price;
        auto discounted_unit_price   = kUnitPrice;
        /*!
            If discount is available at unit price, then calculate discounted price
            Otherwise discounted price will be same as unit hence meaning no discount.
        */
        if (const auto &kArticleDiscount = kOrderItem.article.discount; kArticleDiscount)
        {
            discounted_unit_price = kUnitPrice - XPercentOfY(kArticleDiscount->DiscountPercentage(kUnitPrice, kOrderItem.quantity), kUnitPrice);    
        }
        // Calculate order price for the order item, considering item and order-level discounts
        auto order_price = discounted_unit_price * kOrderItem.quantity;
        if (kOrderItem.discount)
        {
            order_price -= XPercentOfY(kOrderItem.discount->DiscountPercentage(kOrderItem.article.price, kOrderItem.quantity), order_price);
        }
        total_price += order_price;
    }

    // Apply order-level discount if available
    if (customer.order.discount)
    {
        total_price -= customer.order.discount->DiscountPercentage(std::size(customer.order.order_items), total_price);
    }
    // Apply customer-level discount if available
    if (customer.discount)
    {
        total_price -= XPercentOfY(customer.discount->DiscountPercentage(total_price, 0), total_price);
    }
    return total_price;
}

int main()
{
    const auto kFixedDiscount10Percent = FixedDiscount{ 10 };
    const auto kVolumeDiscount         = VolumeDiscount{ 10, 15 };
    const auto kPriceDiscount          = PriceDiscount{ 100, 5 };
    const auto kAmountDiscount         = AmountDiscount{ 100, 5 };

    const auto kShirt = Article{ 1, "Shirt", 5, nullptr };
    const auto kPants = Article{ 2, "Pant", 15, &kFixedDiscount10Percent };
    const auto kBook  = Article{ 3, "Book", 10, nullptr };

    const auto kCustomer1 = Customer{ 100, "Usama", CustomerOrder{ { OrderItem{ kShirt, 1, nullptr } }, nullptr } , nullptr };
    const auto kCustomer2 = Customer{ 101, "Javeria", CustomerOrder{ { OrderItem{ kPants, 1, nullptr } }, nullptr } , nullptr };
    const auto kCustomer3 = Customer{ 102, "John", CustomerOrder{ { OrderItem{ kBook, 15, &kVolumeDiscount } }, nullptr } , nullptr };

    cout << CaclulateTotalPriceFor(kCustomer1) << "\n";
    cout << CaclulateTotalPriceFor(kCustomer2) << "\n";
    cout << CaclulateTotalPriceFor(kCustomer3) << "\n";
    return 0;
}