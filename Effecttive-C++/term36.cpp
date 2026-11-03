#include <iostream>

// Effective C++ 第三版 Item 36：绝不重新定义继承而来的 non-virtual 函数。
// 非虚调用按静态类型绑定；派生类同名定义会使“同一对象”出现两种行为。
// 基类若想允许派生变化，应从一开始把该操作设计为 virtual，并明确其覆盖契约。
// 否则，派生类应使用另一个名字表达扩展能力，避免伪装成覆盖。
// 这不是说派生类不能提供更多成员，而是不能让同名 non-virtual 产生多态错觉。
// 调用点若只持有 Base&，它只能依赖 Base 的静态接口契约。
class Base {
public:
    void identify() const { std::cout << "Base 身份\n"; }
};

class Derived : public Base {
public:
    // 不定义 identify：所有观察角度都保持 Base 的非虚契约。
    void special() const { std::cout << "Derived 的额外能力\n"; }
};

void inspect(const Base& object) {
    object.identify(); // 非虚函数永远按 Base 静态绑定。
}

int main() {
    Derived derived;
    derived.identify();
    inspect(derived);
    derived.special();

    // 若 Derived 另写同名 identify，inspect(derived) 仍调用 Base::identify，容易误导读者。
}
