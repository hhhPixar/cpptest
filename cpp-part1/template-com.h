#ifndef CPP_PART1_TEMPLATE_COMPLEX_H
#define CPP_PART1_TEMPLATE_COMPLEX_H

// 类模板在使用时才以具体类型实例化，例如 Complex<double>。
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

#endif
