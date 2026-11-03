#include <iostream>
#include <string>

// Effective C++ 第三版 Item 28：避免返回 handles 指向对象内部成分。
// 返回内部 string 的引用会让调用者在对象销毁后持有悬空 handle，
// 也可能在非 const 情况下破坏封装；按值返回得到独立副本。
class User {
public:
    explicit User(std::string name) : name_(std::move(name)) {}

    std::string name() const { return name_; } // 返回值与 User 生命周期无关。

private:
    std::string name_;
};

std::string copyName() {
    User temporary("临时用户");
    return temporary.name(); // temporary 离开作用域后，副本仍然有效。
}

int main() {
    User user("小林");
    std::string saved = user.name();
    saved = "已修改的副本"; // 改的是副本，不会改 User 的内部数据。

    std::cout << "对象内: " << user.name() << '\n';
    std::cout << "副本: " << saved << '\n';
    std::cout << "临时对象结果: " << copyName() << '\n';

    // const std::string& risky() const { return name_; } // 该设计暴露生命周期耦合。
}
