#include <iostream>
#include <utility>

// 第 2 章实验：复制构造用于“从同类型已有对象初始化新对象”。
// 在 C++14 中，某些返回值复制可以被优化消除；所以本例只观察语义触发点，
// 不把某次输出次数当作跨编译器、跨优化级别都固定的 ABI/实现承诺。
class Ticket {
public:
    explicit Ticket(int id) : id_(id) {
        std::cout << "value ctor: " << id_ << '\n';
    }

    Ticket(const Ticket& other) : id_(other.id_) {
        std::cout << "copy ctor: " << id_ << '\n';
    }

    Ticket(Ticket&& other) noexcept : id_(other.id_) {
        std::cout << "move ctor: " << id_ << '\n';
    }

    int id() const { return id_; }

private:
    int id_;
};

void takeByValue(Ticket ticket) {
    std::cout << "parameter id: " << ticket.id() << '\n';
}

Ticket makeTicket(int id) {
    Ticket local(id);
    return local;  // 可发生 NRVO；若未消除，优先考虑移动构造。
}

int main() {
    Ticket original(7);
    Ticket direct = original;  // 触发复制构造：新对象由 existing object 初始化。

    takeByValue(original);     // 按值形参也是一个独立对象，触发复制构造。

    Ticket returned = makeTicket(9);
    std::cout << "direct=" << direct.id() << ", returned=" << returned.id() << '\n';

    // 引用只绑定已有对象，不创建 Ticket，因此不会调用复制构造。
    const Ticket& alias = original;
    std::cout << "reference id: " << alias.id() << '\n';
}
