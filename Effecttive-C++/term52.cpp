#include <cstddef>
#include <iostream>
#include <new>
#include <stdexcept>

// Item 52：写了 placement new 也要写 placement delete。
// 若构造函数抛异常，编译器会查找参数列表相同的 placement delete 来回收 placement new 得到的内存。
class Widget {
public:
    static void* operator new(std::size_t size, int tag) {
        std::cout << "placement new，tag=" << tag << '\n';
        return ::operator new(size);
    }
    static void operator delete(void* p, int tag) noexcept {
        std::cout << "placement delete，tag=" << tag << '\n';
        ::operator delete(p);
    }
    Widget() { throw std::runtime_error("模拟构造失败"); }
};

int main() {
    try { new (7) Widget; }
    catch (const std::exception& e) { std::cout << "捕获：" << e.what() << '\n'; }
}
// 这里的 int tag 是自定义 placement 参数，不是标准 void* placement new。
// 对应的 placement delete 必须具有 void* 加相同 int 参数的形式。
// 构造完成后，delete 表达式不会自动调用这种 placement delete；它仅处理构造失败。
// 若构造成功，资源的销毁协议应由调用方另行明确设计。
// 运行输出先显示分配，再显示因异常触发的配对回收，最后才进入 catch。
// 这避免构造函数异常时泄漏 operator new 已获得的原始内存。
// 不要把 placement delete 当作普通 delete 的重载替代品。
