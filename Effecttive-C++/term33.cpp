#include <iostream>

// Effective C++ 第三版 Item 33：避免遮掩继承而来的名称。
// 派生类声明同名函数会遮掩基类的全部同名重载，而不只是签名相同的那一个。
// 这条规则发生在重载决议之前，所以编译器不会自动把 Base::draw(double) 加回来。
// 若只想暴露某一个基类版本，也可用转发函数做更精确的控制。
// 本例使用 `using`，它不复制实现，也不会改变 Base::draw(double) 的语义。
// 先理解名称查找，再讨论 virtual 覆盖；二者是不同层次的规则。
// 在大型继承层次中，显式写出 using 可避免接口在重构时悄悄消失。
// 这也是编译期接口设计的一部分。
// 因此，派生类新增重载前应先检查同名基类函数是否仍要供客户调用。
class Base {
public:
    void draw(int size) { std::cout << "Base::draw(int)=" << size << '\n'; }
    void draw(double scale) { std::cout << "Base::draw(double)=" << scale << '\n'; }
};

class Derived : public Base {
public:
    using Base::draw; // 恢复基类重载集合，避免名称遮掩。
    void draw(int size) { std::cout << "Derived::draw(int)=" << size << '\n'; }
};

int main() {
    Derived picture;
    picture.draw(3);    // 调用派生类版本。
    picture.draw(0.5);  // 若没有 using Base::draw，此处会错误匹配到 int 版本。

    // using 声明表达“我有意保留这些基类接口”，而不是偶然依赖查找规则。
}
