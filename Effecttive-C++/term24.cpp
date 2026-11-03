#include <iostream>

// Effective C++ 第三版 Item 24：若所有参数皆需类型转换，请为此采用 non-member 函数。
// 成员运算符只允许隐式转换右侧参数；非成员函数允许转换作用于两侧。
// 是否允许隐式构造是独立设计取舍；本例只为清晰观察运算符的查找与转换规则。
// 生产接口还应避免过度宽松的转换，以免引入歧义。
// 此例中 Amount 的构造函数可访问，正是为了使两个 int 操作数均能参与重载决议。
// 若金额还带货币种类，转换规则就应更严格，并在接口层处理汇率与舍入。
// 条款核心仍是：需要对称转换的二元操作，通常应是 non-member。
class Amount {
public:
    Amount(int yuan) : yuan_(yuan) {} // 故意非 explicit，演示 int 到 Amount 的便利转换。
    int yuan() const { return yuan_; }
private:
    int yuan_;
};

// 不是成员函数，故 10 + amount 与 amount + 10 都能先转换为 Amount。
Amount operator+(const Amount& left, const Amount& right) {
    return Amount(left.yuan() + right.yuan());
}

int main() {
    Amount lunch(15);
    const Amount total1 = lunch + 10;
    const Amount total2 = 10 + lunch;
    std::cout << total1.yuan() << " 元, " << total2.yuan() << " 元\n";

    // 若 operator+ 是 Amount 成员，10 + lunch 的左操作数没有成员函数可调用。
}
