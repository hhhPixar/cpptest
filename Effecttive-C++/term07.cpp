// Effective C++（第3版）Item 7：为多态基类声明 virtual 析构函数。
// 只要对象可能经由基类指针删除，基类析构函数就必须是 virtual。
// 否则派生资源不会按动态类型析构，程序行为未定义。
#include <iostream>
#include <memory>

class Shape {
public:
    virtual ~Shape() { std::cout << "destroy Shape\n"; }
    virtual void draw() const = 0;
};

class Circle : public Shape {
public:
    explicit Circle(int radius) : radius_(radius) {}
    ~Circle() override { std::cout << "destroy Circle\n"; }
    void draw() const override { std::cout << "circle radius=" << radius_ << '\n'; }
private:
    int radius_;
};

// 不是为多态设计的值类型，不应无故添加 virtual 函数及虚表开销。
class Point {
public:
    Point(int x, int y) : x_(x), y_(y) {}
    ~Point() = default;
    void print() const { std::cout << "point(" << x_ << ',' << y_ << ")\n"; }
private:
    int x_;
    int y_;
};

int main() {
    std::unique_ptr<Shape> shape(new Circle(5));
    shape->draw();
    // 离开作用域时 unique_ptr 经 Shape* 删除；virtual 析构保证 Circle 先析构。

    Point point(1, 2);
    point.print();
}
