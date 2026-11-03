#include <iostream>
#include <thread>

// 线程入口函数：新线程会从这里开始执行。
void hello() {
    std::cout << "Hello Concurrent World\n";
}

int main() {
    // 构造 std::thread 后，hello 与 main 可以并发运行。
    std::thread worker(hello);

    // join 等待 worker 结束。离开作用域前必须 join 或 detach；
    // 否则仍处于 joinable 状态的 std::thread 会导致 std::terminate。
    worker.join();
}
