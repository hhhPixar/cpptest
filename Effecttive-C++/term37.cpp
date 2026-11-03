#include <iostream>

// Effective C++ 第三版 Item 37：绝不重新定义继承而来的默认参数值。
// 虚函数按动态类型分派，但默认参数按静态类型绑定；两种规则混用会产生意外结果。
// 因而默认值应由基类接口统一给出；派生类若有不同需求，应改用不同函数或显式实参。
// 派生类覆写时不提供默认值，让默认实参只在基类声明处出现一次。
class Printer {
public:
    virtual ~Printer() = default;
    virtual void print(int copies = 1) const {
        std::cout << "基类打印 " << copies << " 份\n";
    }
};

class FastPrinter : public Printer {
public:
    // 不重写默认参数；调用者统一看到 Printer 的默认值 1。
    void print(int copies) const override {
        std::cout << "快速打印 " << copies << " 份\n";
    }
};

int main() {
    FastPrinter fast;
    Printer& interface = fast;
    interface.print(); // 动态分派到 FastPrinter，实现与默认值均清晰一致。
    fast.print(3); // 以派生类静态类型调用时，必须显式给出参数。

    // 若派生类写 copies = 2，则 interface.print() 仍传入 1，造成静态/动态规则交错。
}
