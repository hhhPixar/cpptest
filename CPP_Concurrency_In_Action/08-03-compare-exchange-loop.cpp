#include <atomic>
#include <cassert>
#include <functional>
#include <iostream>
#include <thread>

void increment_with_weak_cas(std::atomic<int>& value, int times) {
    for (int n = 0; n < times; ++n) {
        int expected = value.load(std::memory_order_relaxed);
        // weak CAS 即使 value == expected 也允许 spuriously fail（伪失败）。
        // 所以必须放在循环中：失败时 expected 被更新为当前值，再重新尝试。
        while (!value.compare_exchange_weak(expected, expected + 1,
                                            std::memory_order_relaxed,
                                            std::memory_order_relaxed)) {
            // 此例仅更新同一计数器，不发布其他状态，relaxed 足够。
        }
    }
}

int main() {
    std::atomic<int> total(0);
    constexpr int increments_per_thread = 2000;

    std::thread left(increment_with_weak_cas, std::ref(total), increments_per_thread);
    std::thread right(increment_with_weak_cas, std::ref(total), increments_per_thread);
    left.join();
    right.join();

    const int expected = 2 * increments_per_thread;
    const int actual = total.load(std::memory_order_relaxed);
    assert(actual == expected);
    std::cout << "CAS 循环计数=" << actual << ", 期望=" << expected << '\n';
    // compare_exchange_strong 不会伪失败，但在真正竞争时仍会失败，通常也要重试。
    // weak 在循环内通常更合适；伪失败不是数据竞争，也不改变正确性。
    return 0;
}
