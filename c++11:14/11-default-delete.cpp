#include <iostream>
#include <utility>

// = delete 明确禁止某项操作，错误发生在编译期。
class NoCopy {
public:
    explicit NoCopy(int value = 0) : value_(value) {}
    NoCopy(const NoCopy&) = delete;
    NoCopy& operator=(const NoCopy&) = delete;

    // = default 要求编译器生成通常的移动操作。
    NoCopy(NoCopy&&) = default;
    NoCopy& operator=(NoCopy&&) = default;
    ~NoCopy() = default;

    int value() const { return value_; }

private:
    int value_;
};

int main() {
    NoCopy source(42);
    NoCopy destination(std::move(source));
    // NoCopy copy = destination;  // 错误：复制构造函数已删除。
    std::cout << destination.value() << '\n';
}
