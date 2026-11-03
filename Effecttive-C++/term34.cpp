#include <iostream>

// Effective C++ 第三版 Item 34：区分接口继承和实现继承。
// 纯虚函数只规定接口；普通虚函数提供可继承默认实现；非虚函数规定固定实现。
// 三者都可能出现在抽象基类中，关键在于调用者究竟继承了哪一部分承诺。
// 若默认实现对所有派生类都不合适，应改为纯虚函数，迫使派生类明确选择。
class Notification {
public:
    virtual ~Notification() = default;
    virtual void send() const = 0; // 只继承接口：每种通知必须自行定义发送方式。

    virtual void audit() const {   // 同时继承接口和默认实现。
        std::cout << "记录审计日志\n";
    }

    void validate() const {        // 接口与实现均固定，不参与动态绑定。
        std::cout << "检查收件人\n";
    }
};

class Email : public Notification {
public:
    void send() const override { std::cout << "发送邮件\n"; }
};

int main() {
    Email mail;
    mail.validate();
    mail.send();
    mail.audit(); // Email 选择继承默认的 audit 实现。
}
