#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

// 简化线程池：任务存为 void()，future 负责把结果或异常交回调用方。
class thread_pool {
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable available_;
    bool stopping_ = false;

public:
    explicit thread_pool(unsigned count) {
        for (unsigned i = 0; i < count; ++i) {
            workers_.emplace_back([this] {
                for (;;) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mutex_);
                        available_.wait(lock, [this] { return stopping_ || !tasks_.empty(); });
                        if (stopping_ && tasks_.empty()) return; // 先清空已接收任务再退出。
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    task(); // 在锁外执行，提交者不会被慢任务阻塞。
                }
            });
        }
    }

    template <typename Function>
    auto submit(Function function) -> std::future<decltype(function())> {
        using result_type = decltype(function());
        auto task = std::make_shared<std::packaged_task<result_type()>>(std::move(function));
        std::future<result_type> result = task->get_future();
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (stopping_) throw std::runtime_error("submit after shutdown");
            tasks_.push([task] { (*task)(); });
        }
        available_.notify_one();
        return result;
    }

    ~thread_pool() {
        { std::lock_guard<std::mutex> lock(mutex_); stopping_ = true; }
        available_.notify_all();
        for (auto& worker : workers_) worker.join();
    }
};

int main() {
    thread_pool pool(2);
    std::vector<std::future<int>> results;
    for (int value = 1; value <= 6; ++value)
        results.push_back(pool.submit([value] { return value * value; }));

    int sum = 0;
    for (auto& result : results) sum += result.get();
    std::cout << "sum of squares=" << sum << '\n';
} 
