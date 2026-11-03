#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

// 一个最小的阻塞队列：空队列时消费者睡眠，而不是循环占用 CPU。
class BlockingQueue {
public:
    void push(int value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            values_.push(value);              // 修改共享队列必须持有同一把锁。
        }
        ready_.notify_one();                  // 解锁后通知一个可能正在等待的消费者。
    }

    int pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        // 谓词同时处理“虚假唤醒”和“通知早于 wait”的情况。
        ready_.wait(lock, [this] { return !values_.empty(); });
        int value = values_.front();
        values_.pop();
        return value;
    }

private:
    std::mutex mutex_;
    std::condition_variable ready_;
    std::queue<int> values_;
};

int main() {
    BlockingQueue queue;
    std::vector<int> consumed;

    // 单消费者保证队列 FIFO 顺序也就是最终打印顺序。
    std::thread consumer([&] {
        for (int i = 0; i < 4; ++i)
            consumed.push_back(queue.pop());
    });

    std::thread producer([&] {
        for (int value : {7, 11, 13, 17})
            queue.push(value);
    });

    producer.join();
    consumer.join();                          // join 后可安全读取 consumed。

    std::cout << "消费顺序:";
    for (int value : consumed)
        std::cout << ' ' << value;
    std::cout << "\n";
}
