#include <iostream>
#include <type_traits>

// 第 7 章实验：模板会为实际使用的类型生成（实例化）相应代码。
// 此处没有虚函数和基类指针：调用目标由模板实参在编译期确定，
// 这就是静态多态。接口约定由表达式是否有效来满足（鸭子类型）。
class Celsius {
public:
    explicit Celsius(double value) : value_(value) {}
    double normalized() const { return value_; }
private:
    double value_;
};

class Fahrenheit {
public:
    explicit Fahrenheit(double value) : value_(value) {}
    double normalized() const { return (value_ - 32.0) * 5.0 / 9.0; }
private:
    double value_;
};

template <typename Temperature>
double averageCelsius(const Temperature& first, const Temperature& second) {
    // 每一种 Temperature 都会独立实例化本函数；两次调用没有运行时分派。
    return (first.normalized() + second.normalized()) / 2.0;
}

template <typename Temperature>
void report(const Temperature& first, const Temperature& second) {
    static_assert(std::is_same<decltype(first.normalized()), double>::value,
                  "Temperature 必须提供返回 double 的 normalized()");
    std::cout << "平均摄氏温度: " << averageCelsius(first, second) << '\n';
}

int main() {
    Celsius morning(20.0);
    Celsius noon(26.0);
    report(morning, noon); // 实例化 report<Celsius> 和 averageCelsius<Celsius>。

    Fahrenheit evening(68.0);
    Fahrenheit night(50.0);
    report(evening, night); // 为 Fahrenheit 生成另一组编译期调用代码。

    // 若传入不提供 normalized() 的类型，错误会在编译期报告，而非运行时。
}
