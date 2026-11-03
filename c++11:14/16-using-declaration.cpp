#include <iostream>

class Base {
public:
    void print(int value) const {
        std::cout << "Base::print(int): " << value << '\n';
    }
};

class Derived : public Base {
public:
    // 派生类声明同名函数会隐藏基类的所有同名重载。
    // using 声明把 Base::print(int) 引入当前作用域，恢复这个重载。
    using Base::print;

    void print(const char* text) const {
        std::cout << "Derived::print(const char*): " << text << '\n';
    }
};

int main() {
    Derived object;
    object.print(2024);       // 调用经 using 引入的基类版本。
    object.print("C++14");   // 调用派生类版本。
}
