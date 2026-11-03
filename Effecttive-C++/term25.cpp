#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

// Effective C++ 第三版 Item 25：考虑写出一个不抛异常的 swap 函数。
// 对拥有资源的类型，交换指针/轻量句柄可避免昂贵复制，并应显式承诺 noexcept。
class Buffer {
public:
    explicit Buffer(int first) : values_{first, first + 1, first + 2} {}
    int first() const { return values_.front(); }

    void swap(Buffer& other) noexcept {
        values_.swap(other.values_);
    }

private:
    std::vector<int> values_;
};

// 在类同一命名空间提供非成员 swap，供泛型代码通过 ADL 找到。
void swap(Buffer& left, Buffer& right) noexcept {
    left.swap(right);
}

int main() {
    Buffer a(1);
    Buffer b(10);
    using std::swap; // 保留标准后备，再让 ADL 选择上面的高效重载。
    swap(a, b);
    std::cout << "a=" << a.first() << ", b=" << b.first() << '\n';
}
