// 源码模型 3：以 lower_bound 观察“算法 + 迭代器 + 比较器”的解耦。
#include <functional>
#include <iostream>
#include <vector>

// 适用于随机访问迭代器的精简 lower_bound：
// 返回第一个“不满足 comp(element, value)”的位置，即第一个 >= value 的位置（默认升序）。
template <typename RandomIt, typename T, typename Compare>
RandomIt mini_lower_bound(RandomIt first, RandomIt last, const T& value, Compare comp) {
    // 候选答案始终位于半开区间 [first, first + count) 内。
    // last - first 与 first + step 都要求随机访问迭代器；真实 std::lower_bound
    // 可支持更弱的前向迭代器，但前进 middle 的代价不再是常数。
    auto count = last - first;
    while (count > 0) {
        // step 取左半长度；middle 落在当前候选区间内，不会越过 last。
        const auto step = count / 2;
        RandomIt middle = first + step;

        if (comp(*middle, value)) {
            // middle 小于目标：排除 [first, middle]，候选区间从 middle 的后一位开始。
            first = middle + 1;
            count -= step + 1;
        } else {
            // middle 不小于目标：答案可能正是 middle，保留左半边。
            count = step;
        }
    }
    // 循环结束表示候选区间为空；first 恰好越过所有“小于 value”的元素。
    // 它可能等于 last：这不是错误，表示所有元素都小于目标。
    return first;
}

struct ById {
    int id;
};

int main() {
    std::vector<int> values{1, 3, 3, 7, 9};
    const auto position = mini_lower_bound(values.begin(), values.end(), 3, std::less<int>{});
    std::cout << "first 3 at index: " << (position - values.begin()) << '\n';

    // 比较器被作为策略注入；算法无需知道元素的具体类型。
    std::vector<ById> records{{2}, {5}, {8}};
    const ById key{5};
    const auto record = mini_lower_bound(
        records.begin(), records.end(), key,
        [](const ById& left, const ById& right) { return left.id < right.id; });
    std::cout << "record id: " << record->id << '\n';

    // 前置条件：范围必须按同一个 comp 排好序；否则二分过程的“排除半边”推理不成立。
}
