#include "account.h"
#include <iostream>

// 为静态数据成员提供唯一的定义和初始值。放在 .cpp 中可避免头文件多重定义。
double Account::m_rate = 0.8;

class Stone {
public:
    Stone(double width = 0, double height = 0) : width_(width), height_(height) {}

    // min 的实参是 const 引用，所以比较运算符也必须承诺不修改对象。
    bool operator<(const Stone& other) const { return width_ * height_ < other.width_ * other.height_; }
    double width() const { return width_; }

private:
    double width_;
    double height_;
};

// 返回 const 引用避免复制；只应接收在调用表达式结束后仍然存活的对象。
template <typename T>
const T& smaller(const T& left, const T& right) {
    return left < right ? left : right;
}

int main() {
    Account::set_rate(5.0);              // 推荐通过类名调用静态成员。
    std::cout << Account::rate() << '\n';

    Account account;
    account.set_rate(7.0);               // 语法允许，但仍然修改共享的类数据。
    std::cout << Account::rate() << '\n';

    Stone first(1, 2);
    Stone second(2, 3);
    first = smaller(first, second);
    std::cout << first.width() << '\n';
}
