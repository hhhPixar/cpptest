#include <cassert>
#include <cstdio>
#include <iostream>

// 同一个“三维点”概念可用不同语言机制表达。这里的示例用于观察：
// 数据布局、成员函数调用和虚函数派发分别由哪些对象成分参与。

// 1) C 风格：数据和操作彼此分离；函数通过指针取得结构体数据。
struct CPoint3d {
    float x;
    float y;
    float z;
};

void print(const CPoint3d* point) {
    std::printf("(%g, %g, %g)\n", point->x, point->y, point->z);
}

// 2) 普通 C++ 类：非静态成员函数通常不增加每个对象的数据大小；
// 调用时编译器会隐式传递 this 指针。
class Point3dValue {
public:
    Point3dValue(float x = 0.0F, float y = 0.0F, float z = 0.0F)
        : x_(x), y_(y), z_(z) {}

    float x() const { return x_; }
    float y() const { return y_; }
    float z() const { return z_; }

private:
    float x_;
    float y_;
    float z_;
};

std::ostream& operator<<(std::ostream& stream, const Point3dValue& point) {
    return stream << '(' << point.x() << ", " << point.y() << ", " << point.z() << ')';
}

// 3) 继承与虚函数：对象经基类指针或引用访问时，调用按动态类型派发。
class Point {
public:
    explicit Point(float x) : x_(x) {}
    virtual ~Point() = default;

    virtual void test() const {
        std::cout << "Point::test\n";
    }

protected:
    float x_;
};

class Point2d : public Point {
public:
    Point2d(float x, float y) : Point(x), y_(y) {}

    void test() const override {
        std::cout << "Point2d::test\n";
    }

protected:
    float y_;
};

class Point3dObject : public Point2d {
public:
    Point3dObject(float x, float y, float z) : Point2d(x, y), z_(z) {}

    float z() const { return z_; }

    void test() const override {
        std::cout << "Point3dObject::test\n";
    }

private:
    float z_;
};

std::ostream& operator<<(std::ostream& stream, const Point3dObject& point) {
    return stream << "z = " << point.z();
}

// 4) 类模板：类型 T 参与编译期实例化；不同 T 形成不同的类类型。
template <typename T>
class Point3dTemplate {
public:
    Point3dTemplate(T x, T y, T z) : x_(x), y_(y), z_(z) {}

    T x() const { return x_; }
    T y() const { return y_; }
    T z() const { return z_; }

private:
    T x_;
    T y_;
    T z_;
};

// 非类型模板参数 dimension 在编译期确定，因此数组无需动态分配。
template <typename T, int dimension>
class PointVector {
public:
    explicit PointVector(const T (&coordinates)[dimension]) {
        for (int index = 0; index < dimension; ++index) {
            coordinates_[index] = coordinates[index];
        }
    }

    T operator[](int index) const {
        assert(index >= 0 && index < dimension);
        return coordinates_[index];
    }

private:
    T coordinates_[dimension];
};

template <typename T, int dimension>
std::ostream& operator<<(std::ostream& stream, const PointVector<T, dimension>& point) {
    stream << '(';
    for (int index = 0; index < dimension; ++index) {
        if (index != 0) {
            stream << ", ";
        }
        stream << point[index];
    }
    return stream << ')';
}

void dispatchDemo(Point sliced, Point* pointer, Point& reference) {
    // 按值传参发生对象切片：派生类附加部分被丢弃，动态类型成为 Point。
    sliced.test();

    // 指针和引用保留原对象的动态类型，因此走虚函数动态绑定。
    pointer->test();
    reference.test();
}

int main() {
    const CPoint3d cPoint = {1.0F, 2.0F, 3.0F};
    print(&cPoint);

    const Point3dValue valuePoint(4.0F, 5.0F, 6.0F);
    std::cout << valuePoint << '\n';

    Point3dObject objectPoint(7.0F, 8.0F, 9.0F);
    std::cout << objectPoint << '\n';

    const int coordinates[2] = {10, 11};
    const PointVector<int, 2> vectorPoint(coordinates);
    std::cout << vectorPoint << '\n';

    dispatchDemo(objectPoint, &objectPoint, objectPoint);
}
