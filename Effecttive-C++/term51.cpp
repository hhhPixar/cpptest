#include <cstddef>
#include <iostream>
#include <new>

// Item 51：编写 new 和 delete 时需固守常规。
// 派生类经 Base* 删除时，类专属 operator delete 收到的 size 可能不同；不能假设 size 恒等于 sizeof(Base)。
class Base {
public:
    virtual ~Base() = default;
    static void* operator new(std::size_t size) {
        std::cout << "分配请求=" << size << "\n";
        return ::operator new(size);
    }
    static void operator delete(void* p, std::size_t size) noexcept {
        std::cout << "删除时传入大小=" << size << "\n";
        ::operator delete(p); // 无论 size 如何，都将原始块交回全局 delete。
    }
};
class Derived : public Base {
public:
    // 让派生对象显著大于 Base；析构中读取数组，避免编译器把它视为完全无用的演示字段。
    ~Derived() override { std::cout << "Derived payload words=" << (sizeof(extra_) / sizeof(extra_[0])) << "\n"; }
private:
    int extra_[8]{};
};

int main() {
    Base* p = new Derived;
    delete p;
    // 输出体现 sized delete 面对的真实对象大小，而非错误硬编码 Base 大小。
}
// C++14 允许编译器选择 sized delete；因此提供 size 参数版本便于观察。
// 不能把这当成“总是 Base 大小”的证明，派生对象删除会改变传入值。
// 若自行管理不同尺寸的内存池，必须按实际规则正确分派，或转交全局实现。
// 本例直接转交，因此不会由于大小信息的差异造成错误释放。
// 虚析构函数保证经 Base* 删除 Derived 时先完整析构派生对象。
// extra_ 只为制造可见的尺寸差异，不承担业务含义。
// 观察输出时，具体字节数会随 ABI、对齐和编译器而变化。
// “固守常规”还包括处理 size 为零以及避免在分配函数中返回空指针。
// 例子刻意不实现数组版本，避免将注意力从标量 new/delete 配对移开。
