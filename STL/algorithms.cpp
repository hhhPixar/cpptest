// 学习目标：STL 算法通过迭代器操作“半开区间 [first, last)”，与容器解耦。
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int main() {
    std::vector<int> values{5, 1, 4, 1, 3, 2};

    // 非修改算法：accumulate 以初值 0 累加，O(N)；find 也是 O(N)。
    // first_four 是首个 4 的位置或 end；两者均不改变 values 当前的插入顺序。
    const int sum = std::accumulate(values.begin(), values.end(), 0);
    const auto first_four = std::find(values.begin(), values.end(), 4);
    std::cout << "sum: " << sum << ", found 4: "
              << (first_four != values.end()) << '\n';

    // sort 原地重排为 1, 1, 2, 3, 4, 5，时间 O(N log N)；
    // 它要求随机访问迭代器，因此可用于 vector/array/deque，不能用于 list。
    std::sort(values.begin(), values.end());
    // binary_search 的前置条件是范围已用同一比较规则排序，查询为 O(log N)。
    std::cout << "binary_search(3): "
              << std::binary_search(values.begin(), values.end(), 3) << '\n';

    // transform 不改变输入 values，而是将每项写入输出迭代器。
    // back_inserter 把赋值转换为 squares.push_back；结果为 1, 1, 4, 9, 16, 25，
    // 尾插摊销 O(1)，扩容时仅 squares 自己的迭代器/指针/引用会失效。
    std::vector<int> squares;
    std::transform(values.begin(), values.end(), std::back_inserter(squares),
                   [](int value) { return value * value; });

    // unique 只把连续重复元素压到前缀并返回新逻辑尾；它不缩短 vector，
    // [unique_end, end) 的值仍可存在但不再有教学上的语义。先排序保证重复值相邻。
    const auto unique_end = std::unique(values.begin(), values.end());
    // erase 才物理删除尾段；vector 的该位置及之后迭代器、指针、引用随之失效。
    values.erase(unique_end, values.end());

    std::cout << "unique sorted values: ";
    std::copy(values.begin(), values.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
}
