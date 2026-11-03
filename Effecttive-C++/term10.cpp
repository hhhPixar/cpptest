// Effective C++（第3版）Item 10：令 operator= 返回一个 reference to *this。
// 返回左操作数引用让 a = b = c 这样的链式赋值按预期从右向左工作。
// 此惯例也与内建类型的赋值行为一致，降低自定义类型的使用成本。
#include <iostream>
#include <string>

class Label {
public:
    explicit Label(std::string text = "") : text_(std::move(text)) {}

    Label& operator=(const Label& other) {
        std::cout << "assign " << other.text_ << " -> " << text_ << '\n';
        text_ = other.text_;
        return *this;
    }
    void print(const char* name) const { std::cout << name << " = " << text_ << '\n'; }

private:
    std::string text_;
};

int main() {
    Label first("first");
    Label second("second");
    Label third("third");

    first = second = third;
    // second = third 返回 second 的引用，随后 first 再从该对象取得值。
    first.print("first");
    second.print("second");
    third.print("third");
}
