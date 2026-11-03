#include <atomic>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <thread>

// join_guard 将“线程必须回收”的规则绑定到作用域。
// 这比在每个 return、异常分支中手写 join 更不易遗漏。
class join_guard {
public:
    explicit join_guard(std::thread& thread) : thread_(thread) {}

    // 守卫不拥有线程对象，因此禁止复制，避免两个守卫重复 join。
    join_guard(const join_guard&) = delete;
    join_guard& operator=(const join_guard&) = delete;

    ~join_guard() {
        // 析构函数不能抛出异常；只在仍可 join 时才回收线程。
        if (thread_.joinable()) {
            thread_.join();
        }
    }

private:
    std::thread& thread_;
};

int main() {
    std::atomic<int> completed{0};

    try {
        std::thread worker([&completed] {
            // atomic 保证主线程在 join 后可安全读取这个完成标记。
            completed.store(1, std::memory_order_release);
        });
        join_guard guard(worker);

        // 模拟创建线程后发生的异常；guard 仍会在栈展开时调用 join。
        throw std::runtime_error("后续初始化失败");
    } catch (const std::exception& error) {
        std::cout << "捕获异常: " << error.what() << '\n';
    }

    // join 建立同步关系，因此此处线程工作已经确定结束。
    const int done = completed.load(std::memory_order_acquire);
    std::cout << "工作线程完成标记: " << done << '\n';
    return done == 1 ? 0 : 1;
}
