#include <iostream>
#include <memory>

struct configuration {
    int generation;
    int limit;
};

int main() {
    std::shared_ptr<configuration> published =
        std::make_shared<configuration>(configuration{1, 100});

    // 原子加载取得的是“对象所有权快照”，因此旧配置可安全地继续使用。
    std::shared_ptr<configuration> reader_snapshot =
        std::atomic_load_explicit(&published, std::memory_order_acquire);
    std::shared_ptr<configuration> replacement =
        std::make_shared<configuration>(configuration{2, 200});
    std::atomic_store_explicit(&published, replacement, std::memory_order_release);

    std::cout << "读者快照: generation=" << reader_snapshot->generation
              << ", limit=" << reader_snapshot->limit << '\n';
    std::shared_ptr<configuration> latest =
        std::atomic_load_explicit(&published, std::memory_order_acquire);
    std::cout << "最新配置: generation=" << latest->generation
              << ", limit=" << latest->limit << '\n';

    // 限制一：原子性只覆盖 shared_ptr 变量，不能让 configuration 的可变字段并发安全。
    // 限制二：C++14 free function 可能使用内部锁，不能据此宣称 lock-free。
    // 限制三：load 与 store 之间不是事务；两个更新者仍须用 mutex、版本检查或 CAS 设计。
    // 因而这里把配置创建完成后视为不可变，并只原子替换整个 shared_ptr。
    const bool snapshots_are_distinct = reader_snapshot->generation == 1 &&
                                        latest->generation == 2;
    std::cout << "适用模式：发布不可变对象快照，而非原地并发修改\n";
    return snapshots_are_distinct ? 0 : 1;
}
