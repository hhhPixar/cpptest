#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// atomic_flag 是最小的原子类型，适合演示不可重入的自旋锁。
class SpinLock {
public:
    SpinLock() { flag_.clear(std::memory_order_relaxed); }

    void lock() {
        // acquire 防止临界区内的读写被重排到成功加锁之前。
        while (flag_.test_and_set(std::memory_order_acquire))
            std::this_thread::yield();         // 竞争时主动让出时间片，避免纯忙等过热。
    }

    void unlock() {
        // release 使本临界区的写入，在下一个成功 acquire 的线程中可见。
        flag_.clear(std::memory_order_release);
    }

private:
    std::atomic_flag flag_;
};

int main() {
    SpinLock lock;
    int total = 0;                             // 仅在 SpinLock 保护下访问。
    const int thread_count = 3;
    const int additions_per_thread = 500;
    std::vector<std::thread> workers;

    for (int i = 0; i < thread_count; ++i) {
        workers.emplace_back([&] {
            for (int n = 0; n < additions_per_thread; ++n) {
                lock.lock();
                ++total;
                lock.unlock();
            }
        });
    }

    for (std::thread& worker : workers)
        worker.join();

    const int expected = thread_count * additions_per_thread;
    std::cout << "自旋锁保护的总数: " << total << "\n";
    std::cout << "期望总数: " << expected << "\n";
    std::cout << (total == expected ? "结果正确\n" : "结果异常\n");
}
