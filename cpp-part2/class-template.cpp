#include <iostream>

// 同一份类定义可为不同元素类型分别实例化。
template <typename T>
class Complex {
public:
    Complex(T real = T{}, T imaginary = T{}) : real_(real), imaginary_(imaginary) {}
    T real() const { return real_; }
    T imag() const { return imaginary_; }

private:
    T real_;
    T imaginary_;
};

int main() {
    Complex<int> integer(1, 2);
    Complex<double> decimal(1.1, 1.2);
    std::cout << integer.real() << ' ' << decimal.imag() << '\n';
}
