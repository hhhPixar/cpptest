#include <iostream>

// Item 44：将与参数无关的代码抽离 templates。
// 每种 T 的 SquareBox 都需要同样的位置和面积计算；把它放进非模板基类避免重复生成。
class BoxGeometry {
protected:
    BoxGeometry(int side, int x, int y) : side_(side), x_(x), y_(y) {}
    int area() const { return side_ * side_; }
    void move(int dx, int dy) { x_ += dx; y_ += dy; }
    int side_, x_, y_;
};
template<class Tag>
class SquareBox : private BoxGeometry {
public:
    SquareBox(int side, int x, int y) : BoxGeometry(side, x, y) {}
    void moveAndPrint() { move(1, 2); std::cout << area() << " @ " << x_ << ',' << y_ << '\n'; }
};
struct Red {}; struct Blue {};

int main() {
    SquareBox<Red>(3, 0, 0).moveAndPrint();
    SquareBox<Blue>(3, 0, 0).moveAndPrint();
}
// 只有真正不依赖 Tag 的状态才适合放到 BoxGeometry。
// 若基类依赖模板参数，仍会为每个实例化产生不同代码。
// 共享基类还能让调试器更清楚地区分通用几何与类型特有逻辑。
// private 继承在此表达“SquareBox 根据几何实现”，不暴露几何接口。
// Red 与 Blue 仅作为编译期标记，使两个模板实例可同时出现。
// 相同输出说明二者复用了相同的非模板算法。
// 性能优化应在测量后进行，避免仅凭模板就认定发生了代码膨胀。
