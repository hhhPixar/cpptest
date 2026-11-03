#include <iostream>

// Item 43：学习处理模板化基类内的名称。
// 当基类依赖模板参数时，普通的未限定查找不会进入基类；需显式说明查找位置。
template<class T>
class Base {
protected:
    void announce() const { std::cout << "Base 为该 T 提供服务\n"; }
};
template<class T>
class Derived : public Base<T> {
public:
    void run() const {
        this->announce(); // this-> 使 announce 成为依赖名称，实例化时再查找。
        Base<T>::announce(); // 这也是等价的明确写法。
    }
};

int main() { Derived<int>{}.run(); }
// this-> 的作用不是运行时行为变化，而是改变模板定义期的名称查找。
// 另一种写法是 using Base<T>::announce; 再直接调用 announce()。
// 明确写 Base<T>::announce() 也可行，但虚函数场景下要注意分派意图。
// 本例并未使用虚函数，专注展示两阶段查找的语法要求。
// 依赖基类是泛型继承中最常见的这类名称查找来源。
// 两次输出分别验证两种明确查找形式都成功调用了基类成员。
// 不要为了回避该规则而把所有基类成员复制到派生类。
// 理解查找规则可保留代码的正常抽象层次。
// 模板实例化为 int 时，Base<int> 才成为实际的直接基类。
// 因而编译器在定义 Derived 时不会盲目假设 announce 一定存在。
// 这是 C++ 模板延迟绑定机制的一部分。
