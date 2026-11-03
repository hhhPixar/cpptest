#include <iostream>

struct FourInts {
    int a;
    int b;
    int c;
    int d;
};

// 顶层 const 不属于按值形参的函数签名；因此不能再写一个 double imag(double)。
double imag(const double& value) { return value; }

int main() {
    double value = 10.0;
    double* pointer = &value;
    double& reference = value;            // 引用必须在定义时绑定，之后不能改绑。

    reference = 20.0;                     // 实际修改的是 value。
    std::cout << "imag: " << imag(value) << '\n';
    std::cout << "value: " << value << ", *pointer: " << *pointer << '\n';
    std::cout << "&value == &reference: " << (&value == &reference) << '\n';

    FourInts object{};
    FourInts& object_reference = object;
    // sizeof(reference) 是被引用对象类型的大小，而不是“引用实现”的大小。
    std::cout << "sizeof(object): " << sizeof(object)
              << ", sizeof(object_reference): " << sizeof(object_reference) << '\n';
}
