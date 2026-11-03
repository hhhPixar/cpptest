#include <iostream>
#include <string>

// Effective C++ 第三版 Item 35：考虑 virtual 函数以外的替代方案。
// Strategy 把变化的折扣算法作为对象组合进 Order，运行期行为无需继承 Order。
class DiscountStrategy {
public:
    virtual ~DiscountStrategy() = default;
    virtual int apply(int price) const = 0;
};

class VipDiscount : public DiscountStrategy {
public:
    int apply(int price) const override { return price * 8 / 10; }
};

class Order {
public:
    Order(int price, const DiscountStrategy& strategy)
        : price_(price), strategy_(strategy) {}
    int finalPrice() const { return strategy_.apply(price_); }
private:
    int price_;
    const DiscountStrategy& strategy_;
};

int main() {
    VipDiscount vip;
    Order order(100, vip);
    std::cout << "应付: " << order.finalPrice() << '\n';

    // 更进一步可用函数指针、std::function 或模板策略替代此处的虚接口。
}
