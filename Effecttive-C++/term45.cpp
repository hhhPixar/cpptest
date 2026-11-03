#include <iostream>
#include <memory>

// Item 45：运用成员函数模板接受所有兼容类型。
// 复制构造函数不能替代成员模板：它只处理完全相同的 SmartPtr<T>。
template<class T>
class SmartPtr {
public:
    explicit SmartPtr(T* p) : p_(p) {}
    template<class U>
    SmartPtr(const SmartPtr<U>& other) : p_(other.get()) {} // U* 可隐式转为 T* 才能通过。
    T* get() const { return p_; }
private:
    T* p_; // 演示非拥有指针，避免示例中的双重释放问题。
};
struct Animal { virtual ~Animal() = default; };
struct Dog : Animal {};

int main() {
    Dog dog;
    SmartPtr<Dog> dogPtr(&dog);
    SmartPtr<Animal> animalPtr(dogPtr);
    std::cout << (animalPtr.get() == &dog ? "向上转换成功" : "失败") << '\n';
}
// 兼容性由 Dog* 到 Animal* 的标准指针转换决定。
// 反方向 Animal 到 Dog 并不安全，因此不会通过这个构造模板。
// 真正的智能指针还要定义所有权、删除器、比较和异常安全等语义。
// 这里特意采用非拥有指针，避免教学片段模糊资源管理责任。
// 成员模板通常还需考虑是否与普通复制构造函数形成重载竞争。
// 编译器生成的同型复制构造仍存在并负责 SmartPtr<Dog> 自身复制。
