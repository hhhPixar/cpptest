#include <iostream>
#include <string>

// 第 2 章实验：用户没有声明构造函数时，编译器会隐式声明默认构造函数。
// 但“有默认构造函数”不等于所有成员都会自动得到有意义的值：内置类型仍应初始化。
struct Trace {
    Trace() { std::cout << "Trace default constructed\n"; }
};

struct ImplicitDefault {
    int number;       // 默认初始化时该 int 的值不确定；本例不读取它。
    Trace trace;      // 编译器生成的默认构造函数会调用成员的默认构造函数。
};

class MemberOrder {
public:
    MemberOrder(int input)
        // 必须按成员声明顺序写；实际初始化也总是 first_、second_ 的声明顺序。
        : first_(input), second_(input + 1) {
        std::cout << "constructor body: first=" << first_
                  << ", second=" << second_ << '\n';
    }

private:
    int first_;
    int second_;
};

class SafeDefaults {
public:
    // 成员初始值设定项优先于类内默认成员初始化器。
    explicit SafeDefaults(std::string name) : name_(name), retries_(3) {}

    void print() const {
        std::cout << name_ << ", retries=" << retries_
                  << ", enabled=" << enabled_ << '\n';
    }

private:
    std::string name_ = "unnamed";
    int retries_ = 1;
    bool enabled_ = true;
};

int main() {
    ImplicitDefault implicit;  // 合成默认构造会构造 trace，但不初始化 number。
    (void)implicit;

    MemberOrder ordered(10);
    SafeDefaults configured("network task");
    configured.print();

    // 规则建议：对基本类型使用成员初始值设定项或类内初始化，避免依赖未初始化值。
}
