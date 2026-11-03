#include <iostream>

// 虚继承用于解决菱形层次中的重复基类问题：Vertex3d 中只有一份 Point2d 子对象。
// 具体的对象布局、虚基类偏移表等均由 ABI 决定，下面只验证语言层面的共享状态。
class Point2d {
public:
    Point2d(float x = 0.0F, float y = 0.0F) : x_(x), y_(y) {}

    void moveTo(float x, float y) {
        x_ = x;
        y_ = y;
    }

    void print() const {
        std::cout << '(' << x_ << ", " << y_ << ')';
    }

protected:
    float x_;
    float y_;
};

class Vertex : public virtual Point2d {
public:
    explicit Vertex(int id = 0) : id_(id) {}

protected:
    int id_;
};

class Point3d : public virtual Point2d {
public:
    explicit Point3d(float z = 0.0F) : z_(z) {}

protected:
    float z_;
};

class Vertex3d : public Vertex, public Point3d {
public:
    Vertex3d(float x, float y, float z, int id)
        // 最派生类负责构造虚基类；Vertex 和 Point3d 对 Point2d 的初始化会被忽略。
        : Point2d(x, y), Vertex(id), Point3d(z) {}

    void print() const {
        Point2d::print();
        std::cout << ", z = " << z_ << ", id = " << id_ << '\n';
    }
};

int main() {
    Vertex3d vertex(1.0F, 2.0F, 3.0F, 7);
    vertex.print();

    // 两条继承路径转换得到的是同一份虚基类状态。
    Point2d& throughVertex = static_cast<Vertex&>(vertex);
    Point2d& throughPoint3d = static_cast<Point3d&>(vertex);
    throughVertex.moveTo(4.0F, 5.0F);
    throughPoint3d.print();
    std::cout << '\n';
}
