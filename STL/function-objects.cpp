// 学习目标：函数对象、比较器和 lambda 如何作为算法的可调用对象。
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

struct ByAbsoluteValue {
    // 严格弱序是排序比较器的契约：不能让 comp(x, x) 为 true，
    // 并且必须保持一致的传递关系；否则 sort 的行为不再有保证。
    bool operator()(int left, int right) const {
        return std::abs(left) < std::abs(right);
    }
};

int main() {
    std::vector<int> values{-4, 2, -1, 3};
    // 函数对象携带“按绝对值”这一策略；sort 原地重排为 -1, 2, 3, -4，O(N log N)。
    // 绝对值相等的元素彼此顺序未指定；需保留等价元素相对次序时应权衡使用 stable_sort。
    std::sort(values.begin(), values.end(), ByAbsoluteValue{});

    // std::greater<int> 是标准函数对象；第二次排序覆盖前一步顺序，状态变为 3, 2, -1, -4。
    // 比较器按值传入，没有捕获外部状态，适合可复用且易推理的排序规则。
    std::sort(values.begin(), values.end(), std::greater<int>{});

    int threshold = 2;
    // lambda 是编译器生成的函数对象；按值捕获使谓词保存调用时的 2，
    // 即使之后修改外部 threshold 也不受影响。count_if 只读取范围，时间 O(N)。
    const auto count = std::count_if(values.begin(), values.end(),
                                     [threshold](int value) { return value > threshold; });
    std::cout << "count > " << threshold << ": " << count << '\n';
}
