#include <iostream>
#include <memory>

// 第 6 章实验：new 表达式先取得存储，再在该存储中构造对象；
// delete 表达式则先调用析构函数，最后归还由 new 取得的存储。
// 不要把对象的“构造/析构”和原始内存的“分配/释放”视为同一步。
class Meter {
public:
    explicit Meter(int value) : value_(value) {
        std::cout << "构造 Meter(" << value_ << ")\n";
    }

    ~Meter() {
        std::cout << "析构 Meter(" << value_ << ")\n";
    }

    void add(int delta) {
        value_ += delta;
    }

    int value() const {
        return value_;
    }

private:
    int value_;
};

int main() {
    std::cout << "--- 自动存储期对象 ---\n";
    {
        Meter local(10);       // 进入块时构造。
        local.add(5);
        std::cout << "local = " << local.value() << '\n';
    }                          // 离开块时自动析构。

    std::cout << "--- 动态存储期对象 ---\n";
    Meter* raw = new Meter(20); // 分配存储并构造对象。
    raw->add(3);
    std::cout << "raw = " << raw->value() << '\n';
    delete raw;                 // 析构对象并释放存储；随后 raw 不可解引用。

    std::cout << "--- 以 RAII 管理动态对象 ---\n";
    std::unique_ptr<Meter> safe(new Meter(30));
    std::cout << "safe = " << safe->value() << '\n';
    // main 返回时 unique_ptr 自动执行等价的 delete，避免遗漏释放。
}
