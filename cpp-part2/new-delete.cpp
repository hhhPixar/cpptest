#include <cstddef>
#include <iostream>

// 演示类专属分配函数的查找规则。
// 这里仍把实际工作交给全局 ::operator new/delete，重点是观察调用时机。
class Foo {
public:
    static void* operator new(std::size_t size) {
        std::cout << "Foo::operator new, size = " << size << '\n';
        return ::operator new(size);
    }

    static void operator delete(void* memory) noexcept {
        std::cout << "Foo::operator delete\n";
        ::operator delete(memory);
    }

    static void* operator new[](std::size_t size) {
        std::cout << "Foo::operator new[], size = " << size << '\n';
        return ::operator new[](size);
    }

    static void operator delete[](void* memory) noexcept {
        std::cout << "Foo::operator delete[]\n";
        ::operator delete[](memory);
    }
};

int main() {
    // 表达式 new Foo 可概念性地拆成两步：
    // 1) 调用合适的 operator new 取得原始内存；2) 在该内存上构造 Foo。
    Foo* one = new Foo;

    // delete 的顺序相反：先析构对象，再调用匹配的 operator delete 回收原始内存。
    delete one;

    constexpr std::size_t count = 9;
    Foo* many = new Foo[count];

    // 必须成对使用 delete[]。把 new[] 得到的指针交给 delete 是未定义行为。
    // 数组分配的额外簿记空间（若有）由实现决定，因此不要假定传入的 size 精确等于 count*sizeof(Foo)。
    delete[] many;
}
