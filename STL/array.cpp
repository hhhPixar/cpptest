#include <algorithm>
#include <array>
#include <cstdlib>
#include <ctime>
#include <iostream>

// 这个示例测量的是进程消耗的 CPU tick（clock），不是严格的墙钟时间。
long get_a_target_long() {
    long target = 0;
    std::cout << "target (0~" << RAND_MAX << "): ";
    std::cin >> target;
    return target;
}

namespace test_array {
constexpr std::size_t array_size = 1000;

void test_array() {
    std::cout << "\ntest_array()...\n";

    // array 的长度属于类型的一部分：这里始终恰有 1000 个 long，
    // {} 先把全部元素置零；它连续存储但不能改变大小。
    std::array<long, array_size> values{};

    const std::clock_t begin = std::clock();
    // 引用 value 直接写回数组槽位；遍历不创建副本，填充为 O(N)。
    for (long& value : values) {
        value = std::rand();
    }
    std::cout << "clock ticks for filling: " << (std::clock() - begin) << '\n';
    // size/front/back/data 均为 O(1)。非空是 front/back 的前置条件；本数组固定非空。
    std::cout << "array.size(): " << values.size() << '\n';
    std::cout << "array.front(): " << values.front() << '\n';
    std::cout << "array.back(): " << values.back() << '\n';
    std::cout << "array.data(): " << static_cast<const void*>(values.data()) << '\n';

    const long target = get_a_target_long();
    const std::clock_t search_begin = std::clock();

    // sort 原地重排 values，之后“填充时的顺序”不再保留；时间 O(N log N)，
    // 对 array 不会发生扩容，现有迭代器/指针仍指向同一元素位置，但元素值已被交换。
    std::sort(values.begin(), values.end());
    // lower_bound 只适用于按同一规则有序的半开区间 [begin, end)，查找 O(log N)。
    // position 指向首个 >= target 的元素，也可能等于 end，故不能直接解引用。
    const auto position = std::lower_bound(values.begin(), values.end(), target);

    std::cout << "sort()+lower_bound(), clock ticks: "
              << (std::clock() - search_begin) << '\n';
    if (position != values.end() && *position == target) {
        std::cout << "found, " << *position << '\n';
    } else {
        std::cout << "not found!\n";
    }
}
} // namespace test_array
