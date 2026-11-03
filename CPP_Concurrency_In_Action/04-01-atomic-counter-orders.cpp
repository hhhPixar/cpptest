#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    std::atomic<int> counter(0);
    const int thread_count = 4;
    const int increments_per_thread = 1000;
    std::vector<std::thread> workers;

    for (int i = 0; i < thread_count; ++i) {
        workers.emplace_back([&] {
            for (int n = 0; n < increments_per_thread; ++n) {
                // 此处只需要“数值不丢失”，不需要借计数器发布其他内存数据。
                // relaxed 仍保证 fetch_add 对 counter 的修改是原子的。
                counter.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    for (std::thread& worker : workers)
        worker.join();

    // join 已建立线程完成同步；此 load 使用 relaxed 对该纯计数场景足够。
    int actual = counter.load(std::memory_order_relaxed);
    int expected = thread_count * increments_per_thread;
    std::cout << "线程数: " << thread_count
              << ", 每线程递增: " << increments_per_thread << "\n";
    std::cout << "计数结果: " << actual << ", 期望: " << expected << "\n";
    std::cout << "memory_order_relaxed 未提供跨变量的可见性保证\n";
    std::cout << (actual == expected ? "原子递增未丢失\n" : "计数异常\n");
    // 线程均已 join，输出完成后 main 返回，示例没有遗留后台线程。
    return actual == expected ? 0 : 1;
}
