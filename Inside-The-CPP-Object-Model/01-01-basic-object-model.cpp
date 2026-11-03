#include <iostream>

// 第 1 章：一个带虚函数、静态数据和普通数据成员的最小 Point。
// 常见实现会把虚函数表指针（vptr）放进每个多态对象；但它的存在、位置和 sizeof
// 都是 ABI 实现细节，标准只保证经由虚函数调用时的动态绑定语义。
class Point {
public:
    explicit Point(float xValue) : x_(xValue) {
        ++pointCount_;
    }

    virtual ~Point() {
        --pointCount_;
    }

    float x() const {
        return x_;
    }

    static int pointCount() {
        // 静态数据成员属于类本身，不属于任何单个 Point 对象。
        return pointCount_;
    }

protected:
    virtual std::ostream& print(std::ostream& stream) const {
        return stream << "Point(" << x_ << ')';
    }

private:
    float x_;              // 非静态数据成员：每个对象各有一份。
    static int pointCount_; // 静态数据成员：整个程序只有一份。
};

int Point::pointCount_ = 0;

int main() {
    Point first(1.5F);
    Point second(2.5F);

    std::cout << "first.x = " << first.x() << '\n';
    std::cout << "live points = " << Point::pointCount() << '\n';
    std::cout << "sizeof(Point) = " << sizeof(Point)
              << " (implementation dependent)\n";
}
