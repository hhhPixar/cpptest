#include <iostream>

// 第 3 章：指向数据成员的指针不是普通地址，它与“所属类类型”绑定。
// ABI 可以把它实现成偏移，也可以附带处理复杂继承的信息；不要读取其位表示。
class Point3d {
public:
    Point3d(float x, float y, float z) : x_(x), y_(y), z_(z) {}

    float x() const { return x_; }
    float y() const { return y_; }
    float z() const { return z_; }

    // 通过成员函数导出成员指针，实际数据仍可以保持 private。
    static float Point3d::*xMember() { return &Point3d::x_; }
    static float Point3d::*yMember() { return &Point3d::y_; }

private:
    float x_;
    float y_;
    float z_;
};

void addTo(Point3d& point, float Point3d::*member, float delta) {
    // .* 把“对象”和“指向其成员的指针”结合，选中的成员可以读写。
    point.*member += delta;
}

int main() {
    Point3d point(1.0F, 2.0F, 3.0F);

    // 类型读作“指向 Point3d 的一个 float 数据成员的指针”。
    float Point3d::*coordinate = nullptr;
    if (coordinate == nullptr) {
        std::cout << "null member pointer cannot select a member\n";
    }

    coordinate = Point3d::yMember();
    std::cout << "selected y = " << point.*coordinate << '\n';
    // ->* 适用于对象指针，和 .* 选择同一个成员。
    std::cout << "same y via pointer = " << (&point)->*coordinate << '\n';

    addTo(point, Point3d::xMember(), 4.0F);
    addTo(point, coordinate, 5.0F);
    std::cout << "after generic updates: (" << point.x() << ", "
              << point.y() << ", " << point.z() << ")\n";
}
