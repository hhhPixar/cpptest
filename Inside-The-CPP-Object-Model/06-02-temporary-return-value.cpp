#include <iostream>
#include <string>
#include <utility>

// 第 6 章实验：临时对象和按值返回的结果拥有完整的生命周期。
// 编译器可实施 RVO/NRVO 或移动构造；这些优化会改变日志次数，
// 因此本程序只验证结果值与资源所有权，绝不把构造次数当作规范。
class Ticket {
public:
    explicit Ticket(std::string text) : text_(std::move(text)) {
        std::cout << "取得票据: " << text_ << '\n';
    }

    Ticket(const Ticket& other) : text_(other.text_) {
        std::cout << "复制票据\n";
    }

    Ticket(Ticket&& other) noexcept : text_(std::move(other.text_)) {
        std::cout << "移动票据\n";
    }

    ~Ticket() {
        std::cout << "归还票据: " << text_ << '\n';
    }

    const std::string& text() const { return text_; }

private:
    std::string text_;
};

Ticket issueTicket(const std::string& owner) {
    Ticket local("给 " + owner + " 的车票");
    // 返回命名局部对象：允许 NRVO；若未消除，通常可改为移动。
    return local;
}

int main() {
    std::cout << "--- 初始化接收返回值 ---\n";
    Ticket ticket = issueTicket("Ada");
    std::cout << "可观察的语义结果: " << ticket.text() << '\n';

    std::cout << "--- 使用纯临时对象 ---\n";
    std::cout << Ticket("短期通行证").text() << '\n';
    // 该完整表达式结束后，临时对象析构；析构的准确时刻由作用域规则保证。

    std::cout << "main 即将结束，ticket 随作用域结束析构\n";
}
