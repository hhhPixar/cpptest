#include <iostream>
#include <memory>

// Item 53：不要轻忽编译器生成的函数。
// 声明一个复制构造函数会抑制隐式移动构造；本例显式 default 两个移动操作恢复移动语义。
class Buffer {
public:
    Buffer() : data_(new int(9)) {}
    ~Buffer() = default; // unique_ptr 负责释放，析构函数不需要手写。
    Buffer(const Buffer& other) : data_(new int(*other.data_)) { std::cout << "深复制\n"; }
    Buffer& operator=(const Buffer& other) { data_.reset(new int(*other.data_)); return *this; }
    Buffer(Buffer&&) noexcept = default;
    Buffer& operator=(Buffer&&) noexcept = default;
    int value() const { return *data_; }
private:
    std::unique_ptr<int> data_;
};

int main() {
    Buffer first;
    Buffer copy = first;
    Buffer moved = std::move(first);
    std::cout << copy.value() << ", " << moved.value() << '\n';
}
// 编译器会在适用时自动生成默认构造、复制、移动和析构等特殊成员函数。
// 一旦用户声明其中某些函数，其他函数的隐式生成规则会发生变化。
// unique_ptr 不可复制，故本类必须明确给出深复制逻辑才能保持可复制。
// default 的移动操作会转移 unique_ptr 所有权，源对象变为有效但值未指定的状态。
// 示例只读取 moved，避免读取已移动 first 的空指针。
// noexcept 移动构造有利于容器在扩容时选择移动而非复制。
// “不要轻忽”意味着先理解生成规则，再决定 default、delete 或自定义。
