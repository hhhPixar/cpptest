#include <iostream>
#include <string>

// Item 47：请使用 traits classes 表现类型信息。
// 迭代器的 value_type 属于类型信息；traits 把“从类型取信息”的规则集中在一个位置。
template<class Iterator>
struct IteratorTraits { using value_type = typename Iterator::value_type; };
template<class T>
struct IteratorTraits<T*> { using value_type = T; }; // 原生指针没有嵌套 typedef，需要偏特化。

template<class Iterator>
void printValueKind(Iterator) {
    using Value = typename IteratorTraits<Iterator>::value_type;
    std::cout << "元素大小：" << sizeof(Value) << " 字节\n";
}
struct IntIterator { using value_type = int; };

int main() {
    int data[] = {1, 2};
    printValueKind(data);       // 使用 T* 偏特化。
    printValueKind(IntIterator{}); // 使用主模板。
}
// traits 的调用方只面对统一的 IteratorTraits<Iterator>::value_type。
// 差异被偏特化吸收，算法不必为指针单独复制一份主体。
// 标准库的 iterator_traits 采用相同思想且覆盖了更多迭代器形式。
// 原生指针并不是类，无法自己声明 value_type，这正是偏特化的价值。
// 使用 typename 是因为该嵌套名称依赖于 Iterator 模板参数。
// sizeof 的输出用于可观测地验证两条萃取路径都得到了 int。
// traits 传递的是编译期类型信息，不增加每个对象的运行时负担。
// 将类型策略从算法中分离，是 Item 47 的核心设计收益。
