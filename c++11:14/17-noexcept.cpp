#include <iostream>
#include <utility>

// noexcept 表示函数承诺不会抛出异常；若承诺被违反，程序会调用 std::terminate。
// 移动构造函数常标记 noexcept，使标准容器扩容时可以放心优先使用移动操作。
class Buffer {
public:
    explicit Buffer(int size) : size_(size) {}

    Buffer(const Buffer&) : size_(0) {
        std::cout << "复制 Buffer\n";
    }

    Buffer(Buffer&& other) noexcept : size_(other.size_) {
        other.size_ = 0;
        std::cout << "移动 Buffer（不会抛异常）\n";
    }

    int size() const noexcept { return size_; }

private:
    int size_;
};

int main() {
    Buffer original(10);
    Buffer moved(std::move(original));
    std::cout << "新对象大小：" << moved.size()
              << ", 原对象大小：" << original.size() << '\n';
}
