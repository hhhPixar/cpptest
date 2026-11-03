#include <future>
#include <iostream>
#include <thread>

// 生产者不需要直接持有 future；promise 是把一次性结果交出去的端点。
void prepare_report(std::promise<int> result) {
    const int completed_items = 5;
    const int points_per_item = 12;
    // 工作函数只拥有自己的局部状态，不会与主线程产生数据竞争。
    const int report_score = completed_items * points_per_item;
    // set_value 建立结果：随后 future::get 可取得这个值。
    // 在同一 promise 上再次 set_value 会抛出 future_error，因此这里仅交付一次。
    result.set_value(report_score);
}

int main() {
    std::promise<int> promise;
    // get_future 每个 promise 只能调用一次，得到与它共享状态配对的 future。
    std::future<int> future = promise.get_future();

    // promise 不可复制但可移动，因此按值移交给工作线程。
    std::thread worker(prepare_report, std::move(promise));

    // 没有轮询：结果未就绪时 get 自动阻塞等待。
    int score = future.get();
    worker.join();                            // 确保线程对象在析构前不再 joinable。

    // 结果计算在工作线程，展示在主线程；两者不需要共享可变变量。
    std::cout << "任务交接结果: " << score << "\n";
    std::cout << "完成项数: " << score / 12 << "，每项分值: 12\n";
    std::cout << "已等待并回收工作线程\n";
    // future 在 get 后失效；需要多次读取时应改用 shared_future。
    std::cout << "future 已取得唯一结果，程序结束\n";
    return 0;                                  // 明确展示独立示例的成功结束路径。
}
