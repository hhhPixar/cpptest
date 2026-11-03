#include <iostream>
#include <memory>
#include <vector>

// 这是可运行的“延迟回收”概念模型，不是无锁容器。
// reader_guard 模拟读者公告：公告非零时，退休对象不得销毁。
class reclamation_model {
    unsigned active_readers_ = 0;
    std::vector<std::unique_ptr<int>> retired_;

public:
    void enter_reader() { ++active_readers_; }
    void leave_reader() { --active_readers_; }

    void retire(std::unique_ptr<int> object) {
        retired_.push_back(std::move(object));
        std::cout << "对象退休，延后回收\n";
    }

    bool try_reclaim() {
        if (active_readers_ != 0) {
            std::cout << "仍有读者公告，暂不释放\n";
            return false;
        }
        retired_.clear(); // 所有已公告读者离开后才实际销毁对象。
        std::cout << "没有读者公告，已回收退休对象\n";
        return true;
    }
};

int main() {
    reclamation_model model;
    std::unique_ptr<int> removed(new int(42));

    // 顺序模拟：读者先公告，移除者只能把对象放入 retired 列表。
    model.enter_reader();
    std::cout << "读者安全读取值: " << *removed << '\n';
    model.retire(std::move(removed));
    const bool reclaimed_too_early = model.try_reclaim();

    // 读者结束临界区并撤销公告，下一次扫描才可销毁对象。
    model.leave_reader();
    const bool reclaimed_after_quiescence = model.try_reclaim();

    // 真实 hazard pointer / epoch 方案需要原子公告、扫描协议和异常路径处理；
    // 关键规则相同：绝不在任何可能持有旧引用的读者存在时 delete。
    return !reclaimed_too_early && reclaimed_after_quiescence ? 0 : 1;
}
