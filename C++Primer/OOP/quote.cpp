#include "quote.h"

#include <iostream>

std::string Quote::isbn() const {
    return bookNo;
}

// Bulk_quote 继承 Quote 的公共接口，并只重写“总价计算”的可变部分。
// 这体现运行时多态：调用者只依赖 Quote&，不需要知道实际折扣类型。
class Bulk_quote : public Quote {
public:
    Bulk_quote() = default;

    // 基类子对象必须由基类构造函数初始化；派生类只初始化自己新增的成员。
    Bulk_quote(const std::string& book, double price,
               std::size_t minimum_quantity, double discount_rate)
        : Quote(book, price), min_qty(minimum_quantity), discount(discount_rate) {}

    // override 让编译器验证该函数确实覆盖了一个 virtual 基类函数，
    // 可防止参数列表拼错而意外形成另一个重载。
    double net_price(std::size_t count) const override {
        if (count >= min_qty) {
            return count * (1 - discount) * price;
        }
        // 未达到门槛时使用原价：总价是“数量 × 单价”，不是“数量 + 单价”。
        return count * price;
    }

private:
    std::size_t min_qty = 0;
    double discount = 0.0; // 例如 0.20 表示八折
};

// 参数使用 Quote 的 const 引用，因此既能接受 Quote，也能接受其任意派生类，
// 并在 item.net_price(n) 处触发虚函数动态绑定。
double print_total(std::ostream& os, const Quote& item, std::size_t count) {
    const double total = item.net_price(count);
    os << "ISBN: " << item.isbn()
       << " # sold: " << count << " total due: " << total << '\n';
    return total;
}
