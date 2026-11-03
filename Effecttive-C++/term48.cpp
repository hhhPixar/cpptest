#include <iostream>
#include <iterator>
#include <vector>

// Item 48：认识 template 元编程。
// 模板元编程在编译期计算；本例用递归模板求 2 的 N 次方，运行时只取结果。
template<unsigned N>
struct PowerOfTwo { enum { value = 2 * PowerOfTwo<N - 1>::value }; };
template<>
struct PowerOfTwo<0> { enum { value = 1 }; };

template<class Iterator>
void advanceBy(Iterator& it, int n, std::random_access_iterator_tag) { it += n; }
template<class Iterator>
void advanceBy(Iterator& it, int n, std::input_iterator_tag) { while (n--) ++it; }
template<class Iterator>
void advanceBy(Iterator& it, int n) {
    // traits + tag dispatch：依据迭代器类别在编译期选择算法。
    advanceBy(it, n, typename std::iterator_traits<Iterator>::iterator_category{});
}

int main() {
    std::vector<int> numbers{10, 20, 30, 40};
    auto it = numbers.begin(); advanceBy(it, 2);
    std::cout << "2^5=" << PowerOfTwo<5>::value << "，第 3 个数=" << *it << '\n';
}
// PowerOfTwo<5>::value 是常量表达式，递归在编译期终止于特化版本。
// 元编程不等同于递归模板；类型选择和 tag dispatch 也是常见形式。
// advanceBy 的第三参数是标签对象，不携带运行时数据。
// 重载决议根据 iterator_category 在编译期选取 + 或循环递增的版本。
// vector 迭代器为随机访问迭代器，故本次执行走 it += n 的高效路径。
// 输入迭代器版本只使用 ++，因此适用范围更宽但复杂度可能更高。
// 现代 C++ 也可用 constexpr 实现部分编译期计算，表达往往更直接。
// 理解经典 TMP 仍有助于阅读标准库及旧版泛型代码。
