#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

int main() {
    // relaxed 适合只关心原子对象自身数值、不借它传递其他数据的场景。
    std::atomic<int> completed(0);
    constexpr int worker_count = 2;

    std::thread first([&] {
        completed.fetch_add(1, std::memory_order_relaxed);
    });
    std::thread second([&] {
        completed.fetch_add(1, std::memory_order_relaxed);
    });
    first.join();
    second.join();

    // join 已保证各线程完成；此处 relaxed load 对纯计数仍足够。
    const int count = completed.load(std::memory_order_relaxed);
    assert(count == worker_count);
    std::cout << "relaxed 计数=" << count << "（不会丢失原子递增）\n";

    int ordinary_payload = 0;
    std::atomic<bool> ready(false);
    std::thread writer([&] {
        ordinary_payload = 123; // 此写入必须由随后 release 操作发布。
        ready.store(true, std::memory_order_release);
    });
    std::thread reader([&] {
        while (!ready.load(std::memory_order_acquire)) {
        }
        // 不能把上面的 release/acquire 改成 relaxed：relaxed 不建立跨变量可见性。
        // 当前 acquire 观察到该 release，故 payload 写 happens-before 此安全读取。
        assert(ordinary_payload == 123);
        std::cout << "acquire/release 已同步普通数据=" << ordinary_payload << '\n';
    });

    writer.join();
    reader.join();
    // 全部共享访问均被原子同步或 join 约束，程序确定结束。
    return 0;
}
