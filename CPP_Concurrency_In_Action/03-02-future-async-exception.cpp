#include <future>
#include <iostream>
#include <stdexcept>

// 任务以普通返回值或异常的形式，把结果交给 future。
int checked_divide(int numerator, int denominator) {
    if (denominator == 0)
        throw std::invalid_argument("除数不能为零");
    return numerator / denominator;
}

int main() {
    // launch::async 强制创建异步任务，避免实现选择 deferred 策略。
    std::future<int> good = std::async(std::launch::async, checked_divide, 42, 6);
    std::future<int> bad = std::async(std::launch::async, checked_divide, 42, 0);

    // wait_for 可用来检查状态；这里任务很小，仍以 get 作为最终同步点。
    if (good.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
        std::cout << "成功任务已提前完成\n";
    else
        std::cout << "成功任务仍在运行，get 会等待\n";

    // get 既等待任务完成，也只允许调用一次。
    std::cout << "42 / 6 = " << good.get() << "\n";

    try {
        // 工作线程抛出的异常不会在线程边界终止程序，而是保存在 shared state。
        std::cout << "42 / 0 = " << bad.get() << "\n";
    } catch (const std::invalid_argument& error) {
        // get 在调用者线程重新抛出原来的异常，便于统一处理失败任务。
        std::cout << "捕获异步异常: " << error.what() << "\n";
    }

    // 两个 future 都已通过 get 消费，异步任务也确定结束。
    std::cout << "主线程正常结束\n";
}
