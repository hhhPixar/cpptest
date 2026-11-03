// Effective C++（第3版）Item 4：确定对象被使用前已先被初始化。
// 成员应放在初始化列表中直接构造，而不是先默认构造、再在构造函数体内赋值。
// 同时，真正的初始化顺序由成员声明顺序决定，不由初始化列表书写顺序决定。
#include <iostream>
#include <string>

class Lesson {
public:
    Lesson(std::string title, int minutes)
        // title_ 被直接以 title 构造；const 成员 minutes_ 也只能在此初始化。
        : title_(std::move(title)), minutes_(minutes), summary_(title_ + " lesson") {}

    void print() const {
        std::cout << summary_ << ": " << minutes_ << " minutes\n";
    }

private:
    // 声明顺序也是初始化顺序，所以 summary_ 可安全地使用已经构造好的 title_。
    std::string title_;
    const int minutes_;
    std::string summary_;
};

class MemberOrder {
public:
    MemberOrder() : first_(10), second_(first_ + 1) {
        // 初始化列表按成员声明顺序书写：first_ 先构造，second_ 因而安全地得到 11。
        // 若故意写成 second_(first_ + 1), first_(10)，实际顺序仍不会改变，只会触发 -Wreorder 警告。
    }
    void print() const { std::cout << "member order: " << first_ << ", " << second_ << '\n'; }
private:
    int first_;
    int second_;
};

int main() {
    Lesson lesson("initialization", 45);
    lesson.print();

    MemberOrder order;
    order.print();

    // 基类和成员也都会先于构造函数体完成初始化，因此体内不宜承担初始化职责。
}
