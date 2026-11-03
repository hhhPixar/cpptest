#include <cstddef>
#include <iostream>
#include <new>

// Item 50：了解 new 和 delete 的合理替换时机。
// 全局替换会影响整个程序；本例仅为类替换以记录该类的分配，保持影响范围可控。
class Logged {
public:
    static void* operator new(std::size_t size) {
        std::cout << "Logged::new 请求 " << size << " 字节\n";
        return ::operator new(size); // 委托全局实现，继承其 new_handler 语义。
    }
    static void operator delete(void* p) noexcept {
        std::cout << "Logged::delete 释放对象\n";
        ::operator delete(p);
    }
    int value = 42;
};

int main() {
    Logged* p = new Logged;
    std::cout << "value=" << p->value << '\n';
    delete p; // 与同一类的 operator delete 配对。
}
// 仅为了记录日志而替换全局 operator new 往往过于侵入。
// 类专属替换也必须遵守 operator new 返回足够对齐内存的约定。
// 调用 ::operator new 而不是递归调用自身，是实现转发时的关键。
// 对应 delete 同样委托全局版本，保证底层配对一致。
// 真正的分配器还要考虑数组、对齐 new、sized delete 和异常路径。
// 记录字节数适用于诊断；不应在生产路径无节制地输出日志。
