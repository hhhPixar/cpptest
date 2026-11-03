#include <iostream>
#include <mutex>
#include <thread>

class synchronized_total {
public:
    void add_batch(int first, int second) {
        // defer_lock 只构造管理对象，尚未锁定；可在准备完成后再决定加锁时机。
        std::unique_lock<std::mutex> lock(mutex_, std::defer_lock);
        std::cout << "批次准备完成，当前持锁状态: "
                  << (lock.owns_lock() ? "已锁定" : "未锁定") << '\n';

        lock.lock();
        total_ += first;

        // unique_lock 支持临时解锁，缩短不需要保护的工作占用临界区时间。
        lock.unlock();
        const int prepared = second * 2;

        lock.lock();
        total_ += prepared;
        // 离开作用域时若仍持锁，unique_lock 自动解锁。
    }

    int get() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return total_;
    }

private:
    mutable std::mutex mutex_;
    int total_ = 0;
};

int main() {
    synchronized_total total;
    std::thread worker([&total] { total.add_batch(10, 5); });
    worker.join();

    const int result = total.get();
    std::cout << "最终总计: " << result << '\n';
    return result == 20 ? 0 : 1;
}
