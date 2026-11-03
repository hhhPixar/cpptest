#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class safe_counter {
public:
    void increment() {
        // 互斥量将“读出、加一、写回”保护成一个不可分割的临界区。
        std::lock_guard<std::mutex> lock(mutex_);
        ++value_;
    }

    int value() const {
        // const 成员也需要加锁读取，所以 mutex_ 声明为 mutable。
        std::lock_guard<std::mutex> lock(mutex_);
        return value_;
    }

private:
    mutable std::mutex mutex_;
    int value_ = 0;
};

int main() {
    safe_counter counter;
    const int thread_count = 4;
    const int increments_per_thread = 10000;
    std::vector<std::thread> workers;

    for (int i = 0; i < thread_count; ++i) {
        workers.emplace_back([&counter] {
            for (int n = 0; n < increments_per_thread; ++n) {
                counter.increment();
            }
        });
    }

    // 在读取最终结果前回收所有写入者，程序确定性结束。
    for (std::thread& worker : workers) {
        worker.join();
    }

    const int expected = thread_count * increments_per_thread;
    const int actual = counter.value();
    std::cout << "计数结果: " << actual << "，期望值: " << expected << '\n';
    return actual == expected ? 0 : 1;
}
