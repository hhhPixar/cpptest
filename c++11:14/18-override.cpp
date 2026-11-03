#include <iostream>

class Base {
public:
    virtual ~Base() = default;
    virtual void print() const { std::cout << "Base\n"; }
};

class Derived : public Base {
public:
    // override 让编译器检查此函数确实覆盖了一个虚函数；拼写或 const 不匹配会报错。
    void print() const override { std::cout << "Derived\n"; }
};

int main() {
    Derived derived;
    Base& base = derived;
    base.print();                         // 虚调用，输出 Derived。
}
