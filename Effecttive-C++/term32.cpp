#include <iostream>

// Effective C++ 第三版 Item 32：确定你的 public 继承塑模出 is-a 关系。
// Circle 可以作为 Shape 使用，因此 public 继承，并能替换任何 Shape 形参。
// 这种关系是行为承诺：任何接受 Shape 的地方都不应因传入 Circle 而失效。
// public 继承不是单纯的代码复用手段；若没有 is-a，应优先考虑组合。
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

class Circle : public Shape {
public:
    explicit Circle(double radius) : radius_(radius) {}
    double area() const override { return 3.1415926 * radius_ * radius_; }
private:
    double radius_;
};

void printArea(const Shape& shape) {
    std::cout << "面积: " << shape.area() << '\n';
}

int main() {
    Circle circle(2.0);
    printArea(circle); // Circle is-a Shape，替换成立。

    // 反例：鸟不一定“is-a 会飞对象”；若基类承诺 fly，企鹅就不宜 public 继承它。
}
