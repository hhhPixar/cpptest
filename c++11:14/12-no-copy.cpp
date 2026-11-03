#include <iostream>
#include <utility>

// 不可复制类型常用于互斥量、文件句柄等“资源只能有一个拥有者”的场景。
// = delete 比把复制构造函数设为 private 更清楚，并且错误会在编译期指出。
class UniqueNumber {
public:
    explicit UniqueNumber(int value) : value_(value) {}

    UniqueNumber(const UniqueNumber&) = delete;
    UniqueNumber& operator=(const UniqueNumber&) = delete;

    // 禁止复制不等于禁止移动：资源所有权仍可转交给另一个对象。
    UniqueNumber(UniqueNumber&&) = default;
    UniqueNumber& operator=(UniqueNumber&&) = default;

    int value() const { return value_; }

private:
    int value_;
};

int main() {
    UniqueNumber source(42);
    UniqueNumber target(std::move(source));
    // UniqueNumber copy(target);  // 编译错误：复制构造函数已被删除。
    std::cout << "移动后的值：" << target.value() << '\n';
}
