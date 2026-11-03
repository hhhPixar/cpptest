#include <iostream>

// const 成员函数承诺不修改对象；其隐式 this 指针为“指向 const 对象的指针”。
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
    const Complex<double> value(1.0, 2.0);
    // const 对象只能调用 const 成员函数。
    std::cout << value.real() << ' ' << value.imag() << '\n';
}
