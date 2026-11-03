#include <iostream>

// Item 39：明智而审慎地使用 private 继承。
// private 继承表示“根据某物实现”，不是公开的 is-a；外部不能把 Timer 转成 TickSource。
class TickSource {
protected:
    void tick() const { std::cout << "底层产生一个 tick\n"; }
};

class Timer : private TickSource {
public:
    void waitOnce() const {
        // 实现类仍可复用受保护成员，且不会暴露整个基类接口。
        tick();
        std::cout << "Timer 完成一次等待\n";
    }
};

int main() {
    Timer timer;
    timer.waitOnce();
    // TickSource& source = timer; // 错误：private 继承不允许这种客户端转换。
}
// 选择 private 继承前，先问能否用成员对象表达关系。
// 若需要访问基类 protected 成员，private 继承可以很简洁。
// 若还需改变可访问性，可用 using 声明有选择地重新公开成员。
// 基类对象并不是 Timer 对象可以被客户端替代的抽象。
// 这正是 private 与 public 继承最重要的语义分界。
// 示例没有动态分配，重点只放在接口可见性上。
// 输出中的两行依次展示实现复用和派生类自己的职责。
