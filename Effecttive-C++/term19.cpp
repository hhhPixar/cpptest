#include <iostream>
#include <string>

// Effective C++ 第三版 Item 19：把类设计看作类型设计。
// 设计一个类不只是选择数据成员；还要定义创建、复制、比较及失败行为。
// 本例的 Temperature 用摄氏度作为唯一内部表示，避免单位状态彼此矛盾。
class Temperature {
public:
    // 显式构造阻止把任意 double 意外当成温度传入接口。
    explicit Temperature(double celsius) : celsius_(celsius) {}

    static Temperature fromFahrenheit(double fahrenheit) {
        return Temperature((fahrenheit - 32.0) * 5.0 / 9.0);
    }

    double celsius() const { return celsius_; }
    double fahrenheit() const { return celsius_ * 9.0 / 5.0 + 32.0; }

    // 类型的相等语义明确为“内部摄氏值相同”。
    bool operator==(const Temperature& other) const {
        return celsius_ == other.celsius_;
    }

private:
    double celsius_;
};

int main() {
    Temperature boiling(100.0);
    Temperature alsoBoiling = Temperature::fromFahrenheit(212.0);

    std::cout << "摄氏: " << boiling.celsius() << '\n';
    std::cout << "华氏转换: " << alsoBoiling.celsius() << '\n';
    std::cout << "相等: " << (boiling == alsoBoiling ? "是" : "否") << '\n';

    // Temperature accidental = 20.0; // explicit 使这种模糊转换不能通过编译。
}
