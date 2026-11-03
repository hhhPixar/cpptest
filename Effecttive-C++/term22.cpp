#include <iostream>
#include <stdexcept>

// Effective C++ 第三版 Item 22：将成员变量声明为 private。
// 私有数据让类能维护不变式，并允许以后改变内部存储而不破坏调用代码。
// 即使某字段暂时无需检查，也应保留这一边界，为未来日志、缓存或同步策略留出余地。
// 访问函数是受控入口，不等于必须为每个成员提供可写 setter。
class Score {
public:
    void set(int value) {
        if (value < 0 || value > 100) {
            throw std::out_of_range("分数必须在 0 到 100 之间");
        }
        value_ = value;
    }

    int value() const { return value_; }
    bool passed() const { return value_ >= 60; }

private:
    int value_ = 0; // 外部无法绕过 set 直接写入 -1 或 101。
};

int main() {
    Score exam;
    exam.set(86);
    std::cout << "分数: " << exam.value()
              << ", 通过: " << (exam.passed() ? "是" : "否") << '\n';

    // exam.value_ = 200; // private 成员不可从外部访问。
}
