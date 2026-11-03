// Effective C++（第3版）Item 17：以独立语句将 newed 对象置入智能指针。
// 同一实参列表中的求值顺序并未完全指定；裸 new 与可能抛异常的操作交错会泄漏。
// 先构造智能指针，再调用可能抛异常的函数，RAII 即可覆盖整个危险区间。
#include <iostream>
#include <memory>
#include <stdexcept>

class Job {
public:
    Job() { std::cout << "Job acquired\n"; }
    ~Job() { std::cout << "Job released\n"; }
};

int priority(bool fail) {
    if (fail) throw std::runtime_error("priority lookup failed");
    return 10;
}

void schedule(std::unique_ptr<Job> job, int level) {
    std::cout << "schedule at level " << level << '\n';
    (void)job;
}

void safe_schedule(bool lookup_fails) {
    // 独立语句已使 Job 受 unique_ptr 管理，后续 priority 抛异常也不会泄漏。
    std::unique_ptr<Job> job(new Job);
    const int level = priority(lookup_fails);
    schedule(std::move(job), level);
}

int main() {
    try {
        safe_schedule(true);
    } catch (const std::exception& error) {
        std::cout << "caught: " << error.what() << '\n';
    }
    std::cout << "Job was released during stack unwinding\n";
}
