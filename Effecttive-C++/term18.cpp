// Effective C++（第3版）Item 18：让接口容易被正确使用，不易被误用。
// 强类型和工厂函数把单位、构造规则编码进接口，避免客户传错裸 int。
// 返回 const 值也阻止了诸如 if (a * b = c) 这样的无意义赋值。
#include <iostream>
#include <stdexcept>

class Month {
public:
    static Month from_number(int value) {
        if (value < 1 || value > 12) throw std::out_of_range("month must be 1..12");
        return Month(value);
    }
    int number() const { return value_; }
private:
    explicit Month(int value) : value_(value) {}
    int value_;
};

class Date {
public:
    Date(int year, Month month, int day) : year_(year), month_(month), day_(day) {
        if (day < 1 || day > 31) throw std::out_of_range("day must be 1..31");
    }
    void print() const { std::cout << year_ << '-' << month_.number() << '-' << day_ << '\n'; }
private:
    int year_;
    Month month_;
    int day_;
};

class Ticket {
public:
    explicit Ticket(int cents) : cents_(cents) {}
    int cents() const { return cents_; }
private:
    int cents_;
};

const Ticket operator+(const Ticket& left, const Ticket& right) {
    return Ticket(left.cents() + right.cents());
}

int main() {
    Date release(2026, Month::from_number(11), 3);
    release.print();

    Ticket adult(1200);
    Ticket child(800);
    Ticket total = adult + child;
    std::cout << "total cents: " << total.cents() << '\n';
    // adult + child = Ticket(1); // 编译错误：operator+ 的临时结果是 const。
}
