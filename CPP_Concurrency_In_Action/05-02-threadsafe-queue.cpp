#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

// 一个最小阻塞队列：队列空时消费者睡眠而非持续轮询。
template <typename T>
class threadsafe_queue {
    std::queue<T> data_;
    mutable std::mutex mutex_;
    std::condition_variable ready_;

public:
    void push(T value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            data_.push(std::move(value));
        } // 必须先解锁，避免被唤醒线程立刻又阻塞在 mutex 上。
        ready_.notify_one();
    }

    T wait_and_pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        // 谓词同时抵御虚假唤醒，并避免“检查后再等待”的丢失通知。
        ready_.wait(lock, [this] { return !data_.empty(); });
        T value = std::move(data_.front());
        data_.pop();
        return value;
    }
};

int main() {
    threadsafe_queue<int> queue;
    constexpr int stop = -1;
    int consumed_sum = 0;

    std::thread consumer([&] {
        for (;;) {
            int value = queue.wait_and_pop();
            if (value == stop) break; // 哨兵保证消费者一定能正常退出。
            consumed_sum += value;
        }
    });

    std::thread producer([&] {
        for (int value = 1; value <= 10; ++value) queue.push(value);
        queue.push(stop); // 所有任务之后提交停止信号，保持 FIFO 语义。
    });

    producer.join();
    consumer.join();
    std::cout << "consumed sum=" << consumed_sum << '\n';
}
