#include <iostream>

// 第 4 章实验：构造和析构期间，虚调用不会分派到尚未构造或已析构的派生部分。
// 因而 Base 的构造/析构函数内调用 report() 时，只会得到 Base::report()。
class Base {
public:
    Base() {
        std::cout << "Base ctor -> ";
        report();
    }

    virtual ~Base() {
        std::cout << "Base dtor -> ";
        report();
    }

    virtual void report() const {
        std::cout << "Base（派生部分此时不可用）\n";
    }
};

class Derived : public Base {
public:
    Derived() {
        std::cout << "Derived ctor -> ";
        report();  // 到此时 Derived 子对象已开始构造，正常动态分派到 Derived。
    }

    ~Derived() override {
        std::cout << "Derived dtor -> ";
        report();  // Derived 部分尚在，仍分派到 Derived。
    }

    void report() const override {
        std::cout << "Derived\n";
    }
};

int main() {
    std::cout << "--- create ---\n";
    Base* object = new Derived;
    std::cout << "--- ordinary virtual call ---\n";
    object->report();  // 完整对象存活时，经 Base* 分派到 Derived。
    std::cout << "--- destroy ---\n";
    delete object;     // 虚析构函数保证先析构 Derived，再析构 Base。
}
