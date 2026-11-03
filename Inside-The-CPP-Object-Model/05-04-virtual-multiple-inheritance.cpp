#include <iostream>

// 第 5 章实验：多重继承及虚继承的基类子对象地址可能不同于最派生对象地址。
// 转换所需的 this 调整、虚基类定位表等是 ABI 边界：只能观察转换结果，
// 绝不能用 reinterpret_cast、固定偏移或 sizeof 推断来跨编译器/库传递对象。
class SharedState {
public:
    explicit SharedState(int value) : value_(value) {}
    void set(int value) { value_ = value; }
    int value() const { return value_; }

private:
    int value_;
};

class Left : public virtual SharedState {
public:
    Left() : SharedState(-1) {}  // 最派生对象构造时，这个虚基初始化会被忽略。
    void leftWork() const { std::cout << "left sees " << value() << '\n'; }
};

class Right : public virtual SharedState {
public:
    Right() : SharedState(-2) {}
    void rightWork() const { std::cout << "right sees " << value() << '\n'; }
};

class Complete : public Left, public Right {
public:
    explicit Complete(int value) : SharedState(value) {}  // 最派生类负责虚基类。
};

int main() {
    Complete object(10);
    Left* left = &object;
    Right* right = &object;
    SharedState* fromLeft = left;
    SharedState* fromRight = right;

    std::cout << "Complete = " << static_cast<void*>(&object) << '\n';
    std::cout << "Left     = " << static_cast<void*>(left) << '\n';
    std::cout << "Right    = " << static_cast<void*>(right) << '\n';
    std::cout << "shared bases identical? " << (fromLeft == fromRight) << '\n';

    fromRight->set(99);  // 两条路径访问同一份虚基类子对象。
    left->leftWork();
    right->rightWork();
}
