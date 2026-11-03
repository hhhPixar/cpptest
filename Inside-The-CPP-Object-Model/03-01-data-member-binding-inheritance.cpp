#include <iostream>

// 第 3 章实验：成员绑定、继承访问与布局观察应分开理解。
// x_、label_ 的访问由编译期静态类型和访问控制决定；对象地址/成员偏移
// 则属于实现和 ABI 的布局细节，下面不把 sizeof 或地址差写成固定结论。
class Shape {
public:
    explicit Shape(int x) : x_(x) {}
    int x() const { return x_; }

protected:
    void shiftX(int delta) { x_ += delta; }

private:
    int x_;  // 派生类不能用名字直接访问；但它仍是 Shape 子对象的一部分。
};

class LabelledShape : public Shape {
public:
    LabelledShape(int x, const char* label) : Shape(x), label_(label) {}

    void moveRight(int delta) {
        // protected 基类操作对派生类可见；这不是对 Shape 私有数据的直接访问。
        shiftX(delta);
    }

    void print() const {
        // x() 在此处静态绑定到 Shape::x；它不是 virtual 函数。
        std::cout << label_ << " at x=" << x() << '\n';
    }

private:
    const char* label_;
};

class ColoredShape : public LabelledShape {
public:
    ColoredShape(int x, const char* label, int color)
        : LabelledShape(x, label), color_(color) {}

    void printColor() const { std::cout << "color=" << color_ << '\n'; }

private:
    int color_;
};

int main() {
    ColoredShape item(5, "button", 42);
    item.moveRight(3);
    item.print();
    item.printColor();

    Shape& baseView = item;
    std::cout << "base view x=" << baseView.x() << '\n';
    std::cout << "sizeof(Shape)=" << sizeof(Shape)
              << ", sizeof(ColoredShape)=" << sizeof(ColoredShape)
              << " (ABI/implementation dependent)\n";

    // public 继承允许从 ColoredShape 向上转型为 Shape；反向转型需额外证明动态类型。
}
