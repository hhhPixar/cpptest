#include <iostream>

// Item 46：需要类型转换时，请为模板定义非成员函数。
// operator* 的两个操作数都应允许转换，因此不能做成 Rational<T> 的成员函数。
template<class T>
class Rational {
public:
    Rational(T numerator, T denominator = 1) : n_(numerator), d_(denominator) {}
    T numerator() const { return n_; }
    T denominator() const { return d_; }
private:
    T n_, d_;
};
template<class T>
Rational<T> doMultiply(const Rational<T>& lhs, const Rational<T>& rhs) {
    return Rational<T>(lhs.numerator() * rhs.numerator(), lhs.denominator() * rhs.denominator());
}

// 对已确定为 Rational<int> 的非模板重载，两个实参都可执行隐式转换。
Rational<int> operator*(const Rational<int>& lhs, const Rational<int>& rhs) {
    return doMultiply(lhs, rhs);
}

int main() {
    Rational<int> half(1, 2);
    Rational<int> result = 3 * half; // 3 转换成 Rational<int>，左右操作数机会相同。
    std::cout << result.numerator() << '/' << result.denominator() << '\n';
}
// 函数模板参数推导不会把 int 先转换为 Rational<int> 再推导 T。
// 所以通用 operator* 模板本身不能直接实现 3 * Rational<int>。
// 此例增加确定类型的非模板重载，展示 Item 的“非成员”转换对称性。
// 实际库常将非成员 operator* 写成类模板的 friend，再配合其他设计处理推导。
// doMultiply 留为模板，以保留同型 Rational 的通用计算代码。
// 分子与分母没有约分，输出 3/2 刻意使算术过程一目了然。
