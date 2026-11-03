#include <iostream>
#include <string>

// Effective C++ 第三版 Item 26：尽可能延后变量定义式的出现时间。
// 延迟构造可避免无用工作，也让对象用最准确的初值直接构造。
// 变量的作用域也随之缩小，读者更容易判断它在何时有效、为何存在。
// 这不是要求把每个声明塞进最深花括号，而是让构造时机服务于真实控制流。
class Trace {
public:
    explicit Trace(std::string name) : name_(std::move(name)) {
        std::cout << "构造: " << name_ << '\n';
    }
    ~Trace() { std::cout << "析构: " << name_ << '\n'; }
private:
    std::string name_;
};

void process(bool hasTask) {
    std::cout << "检查任务\n";
    if (!hasTask) {
        std::cout << "没有任务，不构造工作对象\n";
        return;
    }
    Trace work("实际工作"); // 只在确认需要时才创建。
    std::cout << "处理完成\n";
}

int main() {
    process(false);
    process(true);
}
