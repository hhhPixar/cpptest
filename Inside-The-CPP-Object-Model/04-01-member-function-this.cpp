#include <iostream>

// 第 4 章实验：非静态成员函数在语义上带有一个隐式 this 参数。
// 调用 meter.add(3) 可理解为 Meter::add(&meter, 3)，但 this 的传递和实际
// 调用约定由实现决定，程序只能依赖这里展示的语言语义。
class Meter {
public:
    explicit Meter(int value) : value_(value) {}

    Meter& add(int delta) {
        // this 的类型是 Meter* const：不能改指向，却能借它访问当前对象。
        this->value_ += delta;
        return *this;  // 返回当前对象，支持链式调用。
    }

    int value() const {
        // const 成员函数中的 this 类型是 Meter const* const，不能修改 value_。
        return this->value_;
    }

    void showAddress() const {
        std::cout << "this = " << static_cast<const void*>(this)
                  << ", value = " << value_ << '\n';
    }

    static int twice(int number) {
        // 静态成员函数没有 this，因此不能不经对象直接访问 value_。
        return number * 2;
    }

private:
    int value_;
};

int main() {
    Meter first(10);
    Meter second(100);

    first.add(2).add(3);  // 两次调用的隐式 this 都是 &first。
    first.showAddress();
    second.showAddress();
    std::cout << "first.value = " << first.value() << '\n';
    std::cout << "Meter::twice(7) = " << Meter::twice(7) << '\n';
}
