#include <future>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    std::promise<int> start_signal;
    // shared_future 可复制，多个观察者都能读取同一个共享结果。
    std::shared_future<int> signal = start_signal.get_future().share();
    const int worker_count = 3;
    std::vector<int> seen(worker_count, 0);
    std::vector<std::thread> workers;

    for (int i = 0; i < worker_count; ++i) {
        workers.emplace_back([i, signal, &seen] {
            // 三个任务都在此等待同一次广播，而不是彼此竞争唯一结果。
            int round = signal.get();
            seen[i] = round + i;
        });
    }

    // set_value 只能交付一次；随后唤醒所有等待同一共享状态的 shared_future。
    start_signal.set_value(100);
    for (std::thread& worker : workers)
        worker.join();

    // 每个 shared_future::get 都可重复读取，且不会使共享状态失效。
    const int announced_round = signal.get();
    // 主线程统一输出，避免并发写 cout 造成演示输出交错。
    std::cout << "广播轮次: " << announced_round << "\n";
    std::cout << "各工作者收到:";
    for (int value : seen)
        std::cout << ' ' << value;
    std::cout << "\n";
}
