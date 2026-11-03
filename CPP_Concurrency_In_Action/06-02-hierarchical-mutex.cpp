#include <climits>
#include <iostream>
#include <mutex>
#include <stdexcept>

// 层级锁通过运行时检查，禁止低层锁之后再请求高层锁的反序。
class hierarchical_mutex {
    std::mutex internal_mutex_;
    const unsigned long hierarchy_value_;
    unsigned long previous_hierarchy_value_;
    static thread_local unsigned long this_thread_hierarchy_value_;

    void check_for_hierarchy_violation() const {
        if (this_thread_hierarchy_value_ <= hierarchy_value_)
            throw std::logic_error("mutex hierarchy violated");
    }
    void update_hierarchy_value() {
        previous_hierarchy_value_ = this_thread_hierarchy_value_;
        this_thread_hierarchy_value_ = hierarchy_value_;
    }

public:
    explicit hierarchical_mutex(unsigned long value) : hierarchy_value_(value), previous_hierarchy_value_(0) {}
    hierarchical_mutex(const hierarchical_mutex&) = delete;
    void lock() {
        check_for_hierarchy_violation();
        internal_mutex_.lock();
        update_hierarchy_value();
    }
    void unlock() {
        this_thread_hierarchy_value_ = previous_hierarchy_value_;
        internal_mutex_.unlock();
    }
    bool try_lock() {
        check_for_hierarchy_violation();
        if (!internal_mutex_.try_lock()) return false;
        update_hierarchy_value();
        return true;
    }
};

thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value_(ULONG_MAX);

int main() {
    hierarchical_mutex high_level(10000);
    hierarchical_mutex low_level(5000);

    { // 数值由大到小加锁是允许的。
        std::lock_guard<hierarchical_mutex> high(high_level);
        std::lock_guard<hierarchical_mutex> low(low_level);
        std::cout << "correct order accepted\n";
    }

    try {
        std::lock_guard<hierarchical_mutex> low(low_level);
        std::lock_guard<hierarchical_mutex> high(high_level); // 反序立刻被拒绝。
    } catch (const std::logic_error& error) {
        std::cout << "reversed order rejected: " << error.what() << '\n';
    }
}
