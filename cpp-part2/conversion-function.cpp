#include <iostream>
#include <stdexcept>

class Fraction {
public:
    // 非 explicit 的单实参构造函数允许 int 隐式转换为 Fraction，例如 f + 4。
    Fraction(int numerator, int denominator = 1) : numerator_(numerator), denominator_(denominator) {
        if (denominator == 0) {
            throw std::invalid_argument("denominator must not be zero");
        }
    }

    Fraction operator+(const Fraction& other) const {
        // a/b + c/d = (ad + bc) / bd；不能简单相加分母。
        return Fraction(numerator_ * other.denominator_ + other.numerator_ * denominator_,
                        denominator_ * other.denominator_);
    }

    // 转换函数没有返回类型；目标类型写在 operator 后。
    operator double() const { return static_cast<double>(numerator_) / denominator_; }

    int numerator() const { return numerator_; }
    int denominator() const { return denominator_; }

private:
    int numerator_;
    int denominator_;
};

class ExplicitFraction {
public:
    // explicit 禁止隐式 int -> ExplicitFraction 转换，避免意外转换或重载二义性。
    explicit ExplicitFraction(int numerator, int denominator = 1)
        : numerator_(numerator), denominator_(denominator) {}

    operator double() const { return static_cast<double>(numerator_) / denominator_; }

private:
    int numerator_;
    int denominator_;
};

int main() {
    Fraction fraction(3, 5);
    std::cout << "4 + fraction = " << 4 + fraction << '\n'; // Fraction 隐式转为 double。

    // fraction + 4 会在“4 转 Fraction”与“fraction 转 double”之间产生二义性。
    // 显式构造右操作数即可指明选择 Fraction::operator+。
    Fraction sum = fraction + Fraction(4);
    std::cout << "fraction + Fraction(4) = " << sum.numerator() << '/' << sum.denominator() << '\n';

    ExplicitFraction explicit_fraction(3, 5);
    // ExplicitFraction invalid = 4; // 错误：explicit 构造函数不参与隐式转换。
    std::cout << "explicit fraction as double = " << static_cast<double>(explicit_fraction) << '\n';
}
