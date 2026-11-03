#include <iostream>
#include <vector>

struct Point {
    int x;
    int y;
};

int main() {
    // 花括号初始化可用于内置类型、聚合类型和标准容器。
    int number{42};
    Point point{3, 4};
    std::vector<int> values{1, 2, 3, 4};

    // 花括号初始化禁止窄化转换：int bad{3.14}; 会编译失败。
    std::cout << "number=" << number << " point=(" << point.x << ", " << point.y << ")\n";
    std::cout << "values 的大小=" << values.size() << '\n';
}
