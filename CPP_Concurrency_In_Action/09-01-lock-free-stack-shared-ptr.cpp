#include <iostream>
#include <memory>

// C++14 没有 std::atomic<std::shared_ptr<T>>；标准提供下面这些 free function。
// 它们让“读/写 shared_ptr 对象本身”成为原子动作，且读者取得的副本会持有对象。
struct node {
    int value;
    std::shared_ptr<node> next;

    node(int value_, std::shared_ptr<node> next_ = nullptr)
        : value(value_), next(std::move(next_)) {}
};

int main() {
    std::shared_ptr<node> head;

    // 为了让例子确定性结束，main 依次模拟两个发布者动作。
    // atomic_store 的 release 与随后 atomic_load 的 acquire 构成发布/获取关系。
    std::shared_ptr<node> first = std::make_shared<node>(1);
    std::atomic_store_explicit(&head, first, std::memory_order_release);

    std::shared_ptr<node> observed =
        std::atomic_load_explicit(&head, std::memory_order_acquire);
    // 即使现在发布新头结点，observed 仍拥有 first，访问不会悬空。
    std::shared_ptr<node> second = std::make_shared<node>(2, observed);
    std::atomic_store_explicit(&head, second, std::memory_order_release);

    std::cout << "旧快照值: " << observed->value << '\n';
    std::shared_ptr<node> current =
        std::atomic_load_explicit(&head, std::memory_order_acquire);
    std::cout << "当前链表: " << current->value
              << " -> " << current->next->value << '\n';

    // 正确性来自 shared_ptr 的引用计数：读者副本活着，节点就不会被销毁。
    // 但这不是“真正无锁栈”：shared_ptr 控制块和 atomic_* 实现都可能内部加锁；
    // 而且 load、修改 next、store 不是一个 CAS 推入操作，多个写者会丢失更新。
    std::cout << "shared_ptr 防悬空；不保证算法或实现无锁\n";
    return current->value == 2 && observed->value == 1 ? 0 : 1;
}
