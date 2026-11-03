#include <chrono>
#include <future>
#include <iostream>
#include <numeric>
#include <vector>

long long sequential_sum(const std::vector<int>& values, std::size_t first, std::size_t last) {
    return std::accumulate(values.begin() + first, values.begin() + last, 0LL);
}

int main() {
    // 选择固定输入，使结果可验证；真实程序还应把数据分布纳入评估。
    std::vector<int> values(20000, 1);
    const std::size_t midpoint = values.size() / 2;

    // 成本模型：任务管理开销 + 等待开销，必须相对于每块的有效计算量评估。
    // 若每块只做几个加法，创建 future 通常比计算本身更昂贵。
    // 因此这里仅派生一个任务，并由当前线程计算另一半，避免无界地拆分。
    const auto begin = std::chrono::steady_clock::now();
    auto lower = std::async(std::launch::async, [&] {
        return sequential_sum(values, 0, midpoint);
    });
    // 当前线程不闲置等待，而是计算另一块；最后再收集异步结果。
    const long long upper = sequential_sum(values, midpoint, values.size());
    const long long lower_value = lower.get(); // get 也会重新抛出子任务异常。
    const long long total = lower_value + upper;
    const auto elapsed = std::chrono::steady_clock::now() - begin;

    // 输出仅说明本次运行，不把短小样本的时间当作稳定性能结论。
    const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    std::cout << "total=" << total << ", one-task time=" << microseconds << " us\n";
    // 计时值受运行环境影响，适合观察，不适合作为跨机器的固定比较数字。
    std::cout << "Use parallel work only when useful computation dominates task overhead.\n";
    // 在生产代码中可用基准测试和负载数据校准该阈值。
}
