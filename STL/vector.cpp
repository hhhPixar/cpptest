#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

std::string get_a_target_string() {
    long target = 0;
    std::cout << "target (0~" << RAND_MAX << "): ";
    std::cin >> target;
    return std::to_string(target);
}

namespace test_vector {
void test_vector(long count) {
    std::cout << "\ntest_vector()...\n";
    std::vector<std::string> values;
    // reserve 只预留存储、不构造元素，size 仍为 0；容量足够时随后 push_back
    // 不搬迁元素，从而避免此前指针/引用/迭代器因扩容失效。
    values.reserve(static_cast<std::size_t>(count));

    const std::clock_t fill_begin = std::clock();
    for (long index = 0; index < count; ++index) {
        // 尾插摊销 O(1)；若超出 capacity 则可能重分配，所有元素观察者都会失效。
        values.push_back(std::to_string(std::rand()));
    }
    std::cout << "clock ticks for filling: " << (std::clock() - fill_begin) << '\n';
    std::cout << "vector.size(): " << values.size() << '\n';
    // front/back 的前置条件是非空；这里单独返回，避免对空 vector 产生未定义行为。
    if (values.empty()) {
        std::cout << "empty input: no front/back element to inspect\n";
        return;
    }
    // vector 连续存储，data 指向首元素；capacity 是已分配槽位数，不等于元素数量。
    std::cout << "vector.front(): " << values.front() << '\n';
    std::cout << "vector.back(): " << values.back() << '\n';
    std::cout << "vector.data(): " << static_cast<const void*>(values.data()) << '\n';
    std::cout << "vector.capacity(): " << values.capacity() << '\n';

    const std::string target = get_a_target_string();
    {
        const std::clock_t begin = std::clock();
        // find 按当前顺序逐个比较，O(N)；found 是元素位置或 end，查找本身不修改 vector。
        const auto found = std::find(values.begin(), values.end(), target);
        std::cout << "find(), clock ticks: " << (std::clock() - begin) << '\n';
        std::cout << (found != values.end() ? "found: " + *found : "not found!") << '\n';
    }

    {
        const std::clock_t begin = std::clock();
        // 排序改变容器原顺序，O(N log N)，但不改变 vector 的容量；
        // lower_bound 的前置条件正是该范围已按相同字典序排序。
        std::sort(values.begin(), values.end());
        const auto found = std::lower_bound(values.begin(), values.end(), target);
        std::cout << "sort()+lower_bound(), clock ticks: " << (std::clock() - begin) << '\n';
        // lower_bound 仅给出首个“不小于” target 的位置，必须再判等确认命中。
        if (found != values.end() && *found == target) {
            std::cout << "found: " << *found << '\n';
        } else {
            std::cout << "not found!\n";
        }
    }
}
} // namespace test_vector
