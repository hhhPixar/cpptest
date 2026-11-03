#include <iostream>

// 第 4 章实验：成员函数指针必须携带“所属类”和函数类型信息，不能当作普通函数指针。
// 多重继承或虚函数下其内部表示可能需要调整信息；不要转换或假设其二进制布局。
class Calculator {
public:
    explicit Calculator(int value) : value_(value) {}

    void add(int delta) { value_ += delta; }
    int scaled(int factor) const { return value_ * factor; }
    int value() const { return value_; }

private:
    int value_;
};

int main() {
    Calculator calculator(5);

    // 该类型读作：指向 Calculator 的、参数为 int、返回 void 的成员函数。
    void (Calculator::*mutate)(int) = &Calculator::add;
    // const 限定属于成员函数类型的一部分，不能省略。
    int (Calculator::*inspect)(int) const = &Calculator::scaled;

    (calculator.*mutate)(7);       // .*：对象 + 成员函数指针，再进行调用。
    Calculator* pointer = &calculator;
    std::cout << "value = " << pointer->value() << '\n';
    std::cout << "scaled = " << (pointer->*inspect)(3) << '\n';  // ->*：对象指针场景。

    // 空成员函数指针可以比较，但调用它是未定义行为，故调用前应检查。
    mutate = nullptr;
    std::cout << "mutate is null? " << (mutate == nullptr) << '\n';
}
