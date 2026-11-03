#include <iostream>

// 本例观察自动对象的构造/析构顺序。
class Base {
public:
    Base() { std::cout << "Base ctor\n"; }
    ~Base() { std::cout << "Base dtor\n"; }
};

// 创建 Derived 时，必须先构造其 Base 子对象；
// 销毁时严格反过来，先析构 Derived，再析构 Base。
// 若 Base 要通过 Base* 删除派生对象，则析构函数必须声明为 virtual；
// 本例没有这样做，所以只演示栈上 Derived 对象的生命周期。
class Derived : public Base {
public:
    Derived() { std::cout << "Derived ctor\n"; }
    ~Derived() { std::cout << "Derived dtor\n"; }
};

void test() {
    std::cout << "...Create...\n";
    Derived object; // 离开 test 作用域时，自动执行析构。
    std::cout << "...Destroy begins when scope ends...\n";
}

int main() {
    test();
}
