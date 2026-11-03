#include <iostream>

// Effective C++ 第三版 Item 30：透彻了解 inlining 的里里外外。
// inline 只是对编译器的建议，同时影响 ODR：同一定义可放在多个翻译单元。
// 频繁修改的内联函数会要求依赖它的翻译单元重编译，二进制体积也可能增长。
// 因此应基于剖析数据决定，不能把 inline 当作无条件的性能开关。
// `inline` 的语言规则和处理器指令缓存中的“内联展开”是相关却不同的概念。
// 将函数实现放进头文件前，还应权衡封装、构建时间和 ABI 演进成本。
// 小函数有时适合内联，但“很短”本身不是语言保证。
// 即使写了 inline，编译器也可能因调试、递归或优化策略而不展开调用。
inline int square(int value) {
    return value * value;
}

class Counter {
public:
    // 类内定义的成员函数隐式 inline，但这不保证机器码内联。
    int next() { return ++value_; }
private:
    int value_ = 0;
};

int main() {
    Counter counter;
    std::cout << "square(5)=" << square(5) << '\n';
    std::cout << "计数=" << counter.next() << ", " << counter.next() << '\n';

    // inline void recursive() { recursive(); } // 递归函数通常不能完整内联。
    // 是否真正展开必须看优化器输出/性能测量，不能由关键字推断。
}
