#ifndef QUOTE_H
#define QUOTE_H

#include <cstddef>
#include <string>

// 多态基类：表示“按册数计算总价”的报价策略。
class Quote {
public:
    Quote() = default;
    Quote(const std::string& book, double sales_price)
        : bookNo(book), price(sales_price) {}

    // 返回书号；const 保证查询不改变报价对象。
    std::string isbn() const;

    // 基类默认策略是不打折。派生类可 override 该策略。
    virtual double net_price(std::size_t count) const { return count * price; }

    // 多态基类应有 virtual 析构函数：通过 Quote* 删除派生对象时才能正确析构。
    virtual ~Quote() = default;

private:
    std::string bookNo;

protected:
    // protected 允许派生类实现定价策略，但普通调用者不能直接访问价格。
    double price = 1.0;
};

// 前置声明只声明类型名；若要定义继承关系，必须给出完整类定义。
class Bulk_quote;

// final 禁止继续派生。
class Final_base final {};

#endif
