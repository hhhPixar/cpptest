#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <stack>
#include <thread>
#include <vector>

// 空栈错误独立成类型，使调用者能明确处理“没有元素”。
struct empty_stack : std::exception {
    const char* what() const noexcept override { return "empty stack"; }
};

// 所有访问底层 stack 的动作都在同一把 mutex 的保护下完成。
template <typename T>
class threadsafe_stack {
    std::stack<T> data_;
    mutable std::mutex mutex_;

public:
    threadsafe_stack() = default;
    threadsafe_stack(const threadsafe_stack&) = delete;
    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    void push(T value) {
        std::lock_guard<std::mutex> lock(mutex_);
        data_.push(std::move(value));
    }

    std::shared_ptr<T> pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (data_.empty()) throw empty_stack();
        // 先复制/移动到栈外对象，再删除，避免检查和取值之间的竞态。
        std::shared_ptr<T> result = std::make_shared<T>(std::move(data_.top()));
        data_.pop();
        return result;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return data_.empty();
    }
};

int main() {
    threadsafe_stack<int> stack;
    std::vector<std::thread> producers;

    // 两个生产者并发入栈；每个数字唯一，最终数量可预测。
    for (int producer = 0; producer < 2; ++producer) {
        producers.emplace_back([&stack, producer] {
            for (int i = 1; i <= 5; ++i) stack.push(producer * 10 + i);
        });
    }
    for (auto& thread : producers) thread.join();

    int count = 0;
    int sum = 0;
    while (!stack.empty()) {
        std::shared_ptr<int> value = stack.pop();
        ++count;
        sum += *value;
    }

    std::cout << "popped=" << count << ", sum=" << sum << '\n';
    try {
        stack.pop(); // 演示空栈仍由异常而不是未定义行为处理。
    } catch (const empty_stack&) {
        std::cout << "empty stack handled\n";
    }
}
