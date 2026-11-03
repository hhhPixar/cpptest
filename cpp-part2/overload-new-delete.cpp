#include <cstddef>
#include <iostream>
#include <new>

struct ConstructionFailed {};

class Foo {
public:
    Foo() { std::cout << "Foo()\n"; }
    explicit Foo(int) {
        std::cout << "Foo(int), then throw\n";
        throw ConstructionFailed{};
    }

    // 类专属 operator new 负责原始内存；构造成功后 delete 会调用对应的普通 delete。
    static void* operator new(std::size_t size) {
        std::cout << "Foo::operator new\n";
        return ::operator new(size);
    }
    static void operator delete(void* memory) noexcept {
        std::cout << "Foo::operator delete\n";
        ::operator delete(memory);
    }

    // 带额外参数的 new 表达式，在构造函数抛异常时匹配此 placement delete。
    static void* operator new(std::size_t size, std::size_t extra) {
        std::cout << "Foo::operator new with extra " << extra << '\n';
        return ::operator new(size + extra);
    }
    static void operator delete(void* memory, std::size_t) noexcept {
        std::cout << "Foo::placement delete after failed construction\n";
        ::operator delete(memory);
    }
};

int main() {
    try {
        Foo* unused = new (300) Foo(1);
        (void)unused;
    } catch (const ConstructionFailed&) {
        std::cout << "construction failure handled\n";
    }
}
