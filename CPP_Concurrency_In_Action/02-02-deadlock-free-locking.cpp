#include <iostream>
#include <mutex>
#include <thread>

struct account {
    explicit account(int initial) : balance(initial) {}

    std::mutex mutex;
    int balance;
};

// 同时锁定两把锁。std::lock 会采用避免死锁的加锁算法，
// 即使另一个线程传入的账户顺序相反，也不会形成循环等待。
void transfer(account& from, account& to, int amount) {
    std::lock(from.mutex, to.mutex);
    std::lock_guard<std::mutex> from_lock(from.mutex, std::adopt_lock);
    std::lock_guard<std::mutex> to_lock(to.mutex, std::adopt_lock);

    if (from.balance >= amount) {
        from.balance -= amount;
        to.balance += amount;
    }
}

int main() {
    account alice(100);
    account bob(100);

    // 两个转账方向相反，正是手写先后顺序时容易死锁的场景。
    std::thread first(transfer, std::ref(alice), std::ref(bob), 30);
    std::thread second(transfer, std::ref(bob), std::ref(alice), 20);
    first.join();
    second.join();

    // 线程结束后独占读取；总额不变验证两次转账都安全完成。
    const int total = alice.balance + bob.balance;
    std::cout << "Alice: " << alice.balance << ", Bob: " << bob.balance
              << ", 总额: " << total << '\n';
    return total == 200 ? 0 : 1;
}
