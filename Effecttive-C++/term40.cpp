#include <iostream>
#include <string>

// Item 40：明智而审慎地使用多重继承。
// 一个基类定义接口，另一个提供实现；这种“接口 + 实现”是较清晰的 MI 用法。
class Printable {
public:
    virtual ~Printable() = default;
    virtual void print() const = 0;
};
class PrefixWriter {
protected:
    void prefix(const std::string& text) const { std::cout << "[报告] " << text; }
};
class Report : public Printable, private PrefixWriter {
public:
    void print() const override { prefix("多重继承的职责必须彼此独立\n"); }
};

int main() {
    Report report;
    Printable& interface = report; // 仅公开真正的接口基类。
    interface.print();
}
// 多重继承并不天然错误，问题在于职责和对象布局是否被理解。
// 此例的 Printable 是纯接口，不持有状态，降低了继承耦合。
// PrefixWriter 则是私有实现细节，客户不会获得其转换能力。
// 若两个基类都提供同名成员，应在调用点明确限定名称。
// 若出现菱形继承，还需审慎考虑虚继承及其初始化规则。
// 将每个基类的角色保持小而单一，能使 MI 更可维护。
