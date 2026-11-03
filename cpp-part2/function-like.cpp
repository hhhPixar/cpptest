#include <iostream>

// 重载 operator() 的对象称为函数对象（仿函数）。
class Multiply {
public:
    explicit Multiply(int factor) : factor_(factor) {}
    int operator()(int value) const { return factor_ * value; }

private:
    int factor_;
};

int main() {
    Multiply triple(3);
    std::cout << triple(4) << '\n';      // 对象可像函数一样调用。
}
