#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

// 消息是值对象；发送者不直接访问 actor 的私有状态。
struct message {
    enum class kind { add, stop } type;
    int value;
};

int main() {
    std::queue<message> mailbox;
    std::mutex mailbox_mutex;
    std::condition_variable arrived;
    int final_total = 0;

    std::thread actor([&] {
        int total = 0; // 只有 actor 线程读写 total，因此不需要为它加锁。
        for (;;) {
            message next{message::kind::stop, 0};
            {
                std::unique_lock<std::mutex> lock(mailbox_mutex);
                arrived.wait(lock, [&] { return !mailbox.empty(); });
                next = mailbox.front();
                mailbox.pop();
            }
            if (next.type == message::kind::stop) {
                final_total = total; // join 建立同步，主线程随后可以安全读取。
                return;
            }
            total += next.value;
        }
    });

    // 发送顺序固定，stop 放在所有业务消息之后，故 actor 会确定性地收尾。
    for (int value : {3, 4, 5}) {
        { std::lock_guard<std::mutex> lock(mailbox_mutex); mailbox.push({message::kind::add, value}); }
        arrived.notify_one();
    }
    { std::lock_guard<std::mutex> lock(mailbox_mutex); mailbox.push({message::kind::stop, 0}); }
    arrived.notify_one();

    actor.join();
    std::cout << "actor total=" << final_total << '\n';
}
