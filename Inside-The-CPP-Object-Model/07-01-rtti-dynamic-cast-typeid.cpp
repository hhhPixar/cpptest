#include <iostream>
#include <memory>
#include <typeinfo>

// 第 7 章实验：RTTI 需要多态基类（至少一个虚函数）。
// dynamic_cast 根据对象的动态类型安全地下行转换：失败的指针转换返回 nullptr；
// typeid(*pointer) 对多态对象求值，得到对象实际类型的 type_info。
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

class Circle : public Shape {
public:
    explicit Circle(double radius) : radius_(radius) {}
    double area() const override { return 3.1415926 * radius_ * radius_; }
    double radius() const { return radius_; }
private:
    double radius_;
};

class Square : public Shape {
public:
    explicit Square(double side) : side_(side) {}
    double area() const override { return side_ * side_; }
private:
    double side_;
};

void inspect(const Shape& shape) {
    std::cout << "动态类型名（实现定义的展示文本）: " << typeid(shape).name()
              << ", 面积: " << shape.area() << '\n';

    const Circle* circle = dynamic_cast<const Circle*>(&shape);
    if (circle != nullptr) {
        std::cout << "  这是圆，半径为 " << circle->radius() << '\n';
    } else {
        std::cout << "  不是圆，安全转换失败\n";
    }
}

int main() {
    std::unique_ptr<Shape> circle(new Circle(2.0));
    std::unique_ptr<Shape> square(new Square(3.0));
    inspect(*circle);
    inspect(*square);

    // type_info 可比较类型身份；不要依赖 name() 的字符串格式或可移植性。
    // 先绑定引用，也使这里表达的“检查多态对象”更明确。
    const Shape& circleAsShape = *circle;
    std::cout << "circle 是 Circle: "
              << (typeid(circleAsShape) == typeid(Circle)) << '\n';
}
