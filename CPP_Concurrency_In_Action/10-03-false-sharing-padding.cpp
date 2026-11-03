#include <atomic>
#include <iostream>
#include <thread>

// 若两个热点原子量落在同一缓存行，不同核心的写入会互相触发缓存失效。
// alignas(64) 是常见教学用缓存行大小；实际硬件的大小应由平台资料确认。
struct alignas(64) padded_counter {
    std::atomic<long long> value{0};
};

int main() {
    // 对照实验常会把两个 atomic 紧挨着放置；那样正确性不受影响，
    // 但可能形成 false sharing。这里选择隔离布局来表达规避手段。
    padded_counter left;
    padded_counter right;
    const int iterations = 100000;

    // 不计时：调度、CPU 频率、缓存层级和编译器都会影响实测结果。
    // 本程序验证的是并发更新后的确定性计数值，而不是基准测试结论。

    std::thread first([&] {
        for (int i = 0; i < iterations; ++i)
            left.value.fetch_add(1, std::memory_order_relaxed);
    });
    std::thread second([&] {
        for (int i = 0; i < iterations; ++i)
            right.value.fetch_add(1, std::memory_order_relaxed);
    });

    first.join();
    second.join();

    // relaxed 足够：这里只需要原子递增，不用借此发布其他内存的内容。
    std::cout << "left=" << left.value.load(std::memory_order_relaxed)
              << ", right=" << right.value.load(std::memory_order_relaxed) << '\n';
    std::cout << "padding separates independent hot counters; measure on your target machine.\n";
}
