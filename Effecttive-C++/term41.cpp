#include <iostream>

// Item 41：了解隐式接口和编译期多态。
// 模板参数不需要继承共同基类；只要在实例化时支持所使用的表达式即可。
// 这种约束由编译器检查，分派也在编译期完成。
struct Celsius {
    int value;
    int reading() const { return value; }
};
struct Fahrenheit {
    int value;
    int reading() const { return (value - 32) * 5 / 9; }
};

template<class Sensor>
void printCelsius(const Sensor& sensor) {
    // 这里形成隐式接口：Sensor 必须有 const reading()，结果可输出。
    std::cout << sensor.reading() << " C\n";
}

int main() {
    printCelsius(Celsius{23});
    printCelsius(Fahrenheit{77});
}
// 与运行时多态不同，这里没有虚函数表，也没有基类指针。
// Celsius 和 Fahrenheit 互不相关，却都满足调用点所需表达式。
// 不满足时，错误会在模板实例化处报告，这是隐式接口的代价。
// C++20 concepts 能把这种要求写得更显式，但 C++14 仍可理解其本质。
// 模板函数的行为可随具体类型而优化，这是编译期多态的收益。
// 例中 Fahrenheit::reading 将读数转换成相同的输出单位。
