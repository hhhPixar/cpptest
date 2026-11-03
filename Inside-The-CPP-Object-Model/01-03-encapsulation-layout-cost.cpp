#include <iostream>

// 第 1 章实验：比较“数据”和“行为”对单个对象大小的影响。
// 这里的 sizeof 是当前编译器/目标 ABI 的观察结果，不能写成跨平台断言。
// 尤其是 vptr 是否存在、放在哪里、占多少字节，均不由 C++ 标准规定。
struct PlainPoint {
    double x;
    double y;
};

class EncapsulatedPoint {
public:
    EncapsulatedPoint(double xValue, double yValue) : x_(xValue), y_(yValue) {}

    double x() const { return x_; }
    double y() const { return y_; }
    void moveBy(double dx, double dy) { x_ += dx; y_ += dy; }

private:
    // private 改变的是名字访问规则，而不是“每个对象多存一份访问权限”。
    double x_;
    double y_;
};

class PolymorphicPoint {
public:
    PolymorphicPoint(double xValue, double yValue) : x_(xValue), y_(yValue) {}
    virtual ~PolymorphicPoint() = default;
    virtual double lengthHint() const { return x_ + y_; }

private:
    double x_;
    double y_;
};

class CountedPoint {
public:
    CountedPoint() { ++created_; }
    static int created() { return created_; }
    double coordinateSum() const { return x_ + y_; }

private:
    // 静态成员属于类，故不会随对象数目重复存储。
    static int created_;
    double x_ = 0.0;
    double y_ = 0.0;
};

int CountedPoint::created_ = 0;

int main() {
    PlainPoint plain{1.0, 2.0};
    EncapsulatedPoint encapsulated(1.0, 2.0);
    PolymorphicPoint polymorphic(1.0, 2.0);
    CountedPoint first;
    CountedPoint second;

    std::cout << "PlainPoint: " << sizeof(plain) << "\n"
              << "EncapsulatedPoint: " << sizeof(encapsulated) << "\n"
              << "PolymorphicPoint: " << sizeof(polymorphic) << "\n"
              << "CountedPoint: " << sizeof(first) << "\n";
    std::cout << "created (static, not per object): " << CountedPoint::created()
              << ", coordinate sum=" << first.coordinateSum() + second.coordinateSum() << '\n';
    // 调用虚函数既让两个数据成员参与可观察计算，也提醒我们：
    // 语言保证此调用的语义，而不保证通过何种 vtable/vptr 机制完成。
    std::cout << "polymorphic length hint: " << polymorphic.lengthHint() << '\n';
    std::cout << "encapsulated x after move: ";
    encapsulated.moveBy(3.0, 0.0);
    std::cout << encapsulated.x() << '\n';
}
