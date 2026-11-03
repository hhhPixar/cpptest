#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <utility>

// 接收线程对象的函数成为新的“回收责任人”。
void finish_in_another_scope(std::thread worker) {
    std::cout << "线程所有权已转移，接收方负责 join\n";
    worker.join();
}

int main() {
    std::atomic<bool> transfer_done{false};
    std::thread producer([&transfer_done] {
        transfer_done.store(true, std::memory_order_release);
    });

    // std::thread 只可移动：producer 随后不再代表执行线程。
    finish_in_another_scope(std::move(producer));
    std::cout << "原对象仍可 join 吗: "
              << (producer.joinable() ? "是" : "否") << '\n';

    // detach 后线程不能再 join，所以只能访问生命期足够长的独立状态。
    std::atomic<bool> detached_done{false};
    std::thread observer([&detached_done] {
        detached_done.store(true, std::memory_order_release);
    });
    observer.detach();

    // 轮询仅用于这个小示例的确定性收尾；不访问已析构的局部资源。
    while (!detached_done.load(std::memory_order_acquire)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    const bool transferred = transfer_done.load(std::memory_order_acquire);
    std::cout << "已确认转移线程完成: " << (transferred ? "是" : "否") << '\n';
    std::cout << "已确认分离线程完成: 是\n";
    return transferred ? 0 : 1;
}
