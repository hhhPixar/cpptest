#include <array>
#include <deque>
#include <iostream>
#include <mutex>

// 一个队列对应一个工作者；每个队列自己带锁，便于说明所有权边界。
class work_queue {
    std::deque<int> tasks_;
    std::mutex mutex_;

public:
    void push_local(int task) {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks_.push_back(task); // 本地任务从尾端取得。
    }

    bool pop_local(int& task) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (tasks_.empty()) return false;
        task = tasks_.back();
        tasks_.pop_back();
        return true;
    }

    bool steal_oldest(int& task) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (tasks_.empty()) return false;
        task = tasks_.front(); // 窃取者取另一端，减少与所有者的策略冲突。
        tasks_.pop_front();
        return true;
    }
};

int main() {
    // 为使输出确定：主线程按固定次序驱动模型；锁仍展示真实实现所需同步。
    std::array<work_queue, 2> queues;
    queues[0].push_local(1);
    queues[0].push_local(2);
    queues[1].push_local(10);

    int task = 0;
    queues[0].pop_local(task);
    std::cout << "worker 0 runs " << task << '\n';

    // 工作者 0 空闲后，从工作者 1 的相反端窃取最早任务。
    queues[0].pop_local(task); // 取完自己的最后一个任务。
    std::cout << "worker 0 runs " << task << '\n';
    if (queues[1].steal_oldest(task))
        std::cout << "worker 0 steals and runs " << task << '\n';

    // 不创建无限工作循环或后台线程，示例在所有任务耗尽后确定性结束。
    std::cout << "all modeled tasks finished\n";
}
