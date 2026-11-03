#include <iostream>

// 注意：vptr/vtable 是主流编译器常见的实现策略，不是 C++ 标准规定的对象布局。
// 标准真正保证的是：通过基类指针/引用调用 virtual 函数时，会依据对象的动态类型派发。
class A {
public:
    virtual void vfunc1() { std::cout << "A::vfunc1()\n"; }
    virtual void vfunc2() { std::cout << "A::vfunc2()\n"; }
    void func1() { std::cout << "A::func1()\n"; }
    void func2() { std::cout << "A::func2()\n"; }

private:
    int m_data1{};
    int m_data2{};
};

class B : public A {
public:
    void vfunc1() override { std::cout << "B::vfunc1()\n"; }
    // 这里不是 virtual 调用点：它只是隐藏了同名的 A::func2。
    void func2() { std::cout << "B::func2()\n"; }

private:
    int m_data3{};
};

int main() {
    B derived;

    // 对象切片：只用 derived 的 A 基类子对象初始化 copied_base。
    // copied_base 的动态类型就是 A，所以调用 A::vfunc1。
    A copied_base = derived;
    copied_base.vfunc1();

    // 指针指向 A 对象，动态类型仍是 A。
    A* base_ptr = &copied_base;
    base_ptr->vfunc1();

    // 向上转型不复制对象：base_ptr 的静态类型是 A*，实际对象的动态类型是 B。
    // virtual 调用因此派发至 B::vfunc1。
    base_ptr = &derived;
    base_ptr->vfunc1();

    // 调用非虚函数按静态类型绑定，输出 A::func2()。
    base_ptr->func2();
}
