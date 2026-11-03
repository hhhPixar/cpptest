#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

// C++14 没有 stop_token；协作取消由调用方共享的原子标志实现。
void cooperative_worker(const std::atomic<bool>& cancel_requested, int& completed_steps) {
    completed_steps = 0;
    for (int step = 0; step < 20; ++step) {
        // 周期性检查是关键：阻塞操作也应采用可超时的等待，才能及时响应取消。
        if (cancel_requested.load(std::memory_order_acquire)) return;
        ++completed_steps;
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

int main() {
    // 标志的生命周期覆盖 worker；不能在线程仍可能读取时销毁它。
    std::atomic<bool> cancel_requested{false};
    // 该普通整数只由 worker 写；主线程会在 join 后才读取它。
    int completed_steps = 0;
    // 通过 cref/ref 传递引用，避免复制 atomic，也避免将结果写到临时对象。
    std::thread worker(cooperative_worker, std::cref(cancel_requested), std::ref(completed_steps));

    // 由固定时间点发出请求，演示不会依赖不确定的用户输入或无限等待。
    // 取消请求不是强制终止：worker 仍负责释放自身拥有的资源并正常返回。
    std::this_thread::sleep_for(std::chrono::milliseconds(9));
    // release 与 worker 的 acquire 配对，表达请求对之后检查可见。
    cancel_requested.store(true, std::memory_order_release);
    // join 既等待线程结束，也建立对 completed_steps 的安全观察边界。
    worker.join();

    // join 后主线程观察 completed_steps，不再与工作线程并发访问它。
    std::cout << "cancel requested, completed steps=" << completed_steps << '\n';
    std::cout << "cancellation is cooperative: the worker chooses safe check points.\n";
}
