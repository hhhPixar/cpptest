#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

int main() {
    std::atomic<bool> left_ready(false);
    std::atomic<bool> right_ready(false);
    std::atomic<int> observations(0);

    std::thread left([&] {
        // seq_cst 操作除原子性外，还参与所有 seq_cst 操作的单一全局顺序。
        left_ready.store(true, std::memory_order_seq_cst);
        if (right_ready.load(std::memory_order_seq_cst)) {
            observations.fetch_add(1, std::memory_order_seq_cst);
        }
    });
    std::thread right([&] {
        right_ready.store(true, std::memory_order_seq_cst);
        if (left_ready.load(std::memory_order_seq_cst)) {
            observations.fetch_add(1, std::memory_order_seq_cst);
        }
    });

    left.join();
    right.join();
    const int seen = observations.load(std::memory_order_seq_cst);

    // 若两个 load 都读到 false，则全局顺序要求 left store 在 left load 后，
    // 且 right store 在 right load 后；再结合每线程 store 在 load 前，形成矛盾。
    // 因而 seq_cst 保证至少一个线程看到对方的 true，seen 必为 1 或 2。
    assert(seen >= 1 && seen <= 2);
    std::cout << "seq_cst 下至少一次观察到对方发布: " << seen << '\n';

    // seq_cst 是最强、最直观的默认顺序，却不替代协议设计：
    // 它只排序该内存序的原子操作；普通共享数据仍需同步后才能访问。
    return 0;
}
