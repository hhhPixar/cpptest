// 头文件保护：同一翻译单元重复包含本文件时，只保留第一份声明。
#ifndef __COMPLEX__
#define __COMPLEX__

#include <iostream>

// 一个教学用的复数类型。它刻意使用最基础的类、成员函数和运算符重载，
// 便于观察“对象如何提供接口、如何隐藏数据”。
class complex {
public:
    // 构造函数的成员初始化列表直接初始化 re、im。
    // 两个默认实参让 complex()、complex(1) 和 complex(1, 2) 都合法。
    complex(double r = 0, double i = 0) : re(r), im(i) {}

    // const 成员函数承诺不修改当前对象；因此 const complex 也能调用它们。
    // 类内定义的函数通常隐式 inline，适合这种很小的访问器。
    double real() const { return re; }
    double imag() const { return im; }

    // 返回 *this 的引用，既避免一次额外复制，也支持 a += b += c 这样的链式调用。
    complex& operator+=(const complex& rhs);

    // 同一个类的成员函数可访问任意 complex 对象的 private 成员，
    // 这来自“类的访问权限”，并不是 friend 的特殊效果。
    int sum_parts(const complex& value) const {
        return static_cast<int>(value.re + value.im);
    }

private:
    // 实现细节保持 private：调用者只能通过公开接口读写复数。
    double re;
    double im;

    // 非成员 operator+ 需要读取实现细节时，可被精确地授予 friend 权限。
    // friend 本身不等于成员函数，也不会继承 this 指针。
    friend complex& __doapl(complex*, const complex&);
    friend int test_friend(const complex&, const complex&);
};

// 友元函数用于演示“受控地打破访问限制”；生产代码应优先让接口表达需求。
inline int test_friend(const complex& lhs, const complex& rhs) {
    return static_cast<int>(lhs.re + lhs.im + rhs.re + rhs.im);
}

// 将 += 的具体加法抽出。ths 是调用 += 的对象地址；返回 *ths 保持链式接口。
inline complex& __doapl(complex* ths, const complex& rhs) {
    ths->re += rhs.re;
    ths->im += rhs.im;
    return *ths;
}

// 对非 static 成员函数，编译器会传入隐藏的 this 指针；这里显式将它交给辅助函数。
inline complex& complex::operator+=(const complex& rhs) {
    return __doapl(this, rhs);
}

// 非成员访问器让后续的运算符实现对称，也允许保持数据成员私有。
inline double imag(const complex& value) { return value.imag(); }
inline double real(const complex& value) { return value.real(); }

// + 不修改操作数，因此按值返回一个新对象；参数使用 const 引用避免复制。
inline complex operator+(const complex& lhs, const complex& rhs) {
    return complex(real(lhs) + real(rhs), imag(lhs) + imag(rhs));
}
inline complex operator+(double lhs, const complex& rhs) {
    return complex(lhs + real(rhs), imag(rhs));
}
inline complex operator+(const complex& lhs, double rhs) {
    return complex(real(lhs) + rhs, imag(lhs));
}

// 一元 + 保持数值不变；一元 - 返回实部和虚部都取反的新对象。
inline complex operator+(const complex& value) { return value; }
inline complex operator-(const complex& value) {
    return complex(-real(value), -imag(value));
}

// 输出运算符的左操作数必须是流，所以通常写成非成员函数；返回流引用支持连续输出。
inline std::ostream& operator<<(std::ostream& os, const complex& value) {
    return os << '(' << real(value) << ',' << imag(value) << ')';
}

#endif
