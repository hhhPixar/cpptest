#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

// 此对象本身不是原子对象；只能在发布前写、在获取发布信号后读。
struct report {
    int id;
    int score;
};

int main() {
    report shared{0, 0};
    std::atomic<bool> published(false);

    std::thread producer([&] {
        // 普通写入先发生于 release store；此时消费者尚不能读取 shared。
        shared.id = 42;
        shared.score = 97;
        // release 将此前的普通写入“发布”给成功 acquire 此值的线程。
        published.store(true, std::memory_order_release);
    });

    std::thread consumer([&] {
        // acquire 读到 true 前只访问 published（原子对象），因此没有数据竞争。
        while (!published.load(std::memory_order_acquire)) {
            // 教学用短自旋；真实程序通常应使用条件变量或更高层协议。
        }

        // acquire 读到 producer 的 release 写入：release synchronizes-with acquire。
        // 所以 producer 对 shared 的写 happens-before 下列普通读取，读取是安全的。
        const int id = shared.id;
        const int score = shared.score;
        assert(id == 42 && score == 97);
        std::cout << "已获取发布的报告: id=" << id << ", score=" << score << '\n';
    });

    producer.join();
    consumer.join();
    // 线程均已结束；不会遗留后台工作，也没有未同步的 shared 访问。
    return 0;
}
