#include <iostream>

// Effective C++ 第三版 Item 21：必须返回对象时，别妄想返回其 reference。
// 返回局部对象的引用会悬空；按值返回让调用者获得独立、可安全使用的结果。
// 不要返回指向栈上局部对象的引用，也不要返回 new 出来的对象引用来转嫁释放责任。
// 乘积没有天然“所属对象”，因此它应是一个新值，而不是借用 left 或 right 的别名。
class Fraction {
public:
    Fraction(int numerator, int denominator)
        : numerator_(numerator), denominator_(denominator) {}

    int numerator() const { return numerator_; }
    int denominator() const { return denominator_; }

private:
    int numerator_;
    int denominator_;
};

// 返回值对象在 C++14 中可享受 RVO 或移动；正确性优先于猜测复制成本。
Fraction multiply(const Fraction& left, const Fraction& right) {
    return Fraction(left.numerator() * right.numerator(),
                    left.denominator() * right.denominator());
}

int main() {
    const Fraction half(1, 2);
    const Fraction third(1, 3);
    const Fraction sixth = multiply(half, third);
    std::cout << sixth.numerator() << '/' << sixth.denominator() << '\n';

    // const Fraction& bad = multiply(half, third); // 虽可绑定，但不应借此设计返回引用。
}
