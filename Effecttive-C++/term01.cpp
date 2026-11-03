// Effective C++（第3版）Item 1：视 C++ 为一个语言联邦。
// 本程序把过程式、面向对象、泛型和 STL 四种子语言放在一个小实验里。
// 它们共享语法，却各有适用的设计规则，不能只用一种思维处理全部代码。
#include <iostream>
#include <string>
#include <vector>

// 过程式部分：值传递适合小型标量，函数的输入输出直接明了。
int square(int value) { return value * value; }

// 面向对象部分：不变量由类维护，外界经由行为而不是公开数据访问状态。
class Counter {
public:
    void add(int value) { total_ += value; }
    int total() const { return total_; }
private:
    int total_ = 0;
};

// 泛型部分：算法针对“可比较类型”工作，不必为 int 和 string 各写一份。
template <class T>
const T& larger(const T& left, const T& right) {
    return left < right ? right : left;
}

int main() {
    std::cout << "procedural square: " << square(6) << '\n';

    Counter counter;
    counter.add(3);
    counter.add(4);
    std::cout << "object total: " << counter.total() << '\n';

    std::cout << "template larger: " << larger(std::string("Ada"), std::string("Bjarne")) << '\n';

    // STL 容器负责内存与元素生命周期；客户端只表达“收集若干值”。
    std::vector<int> values{2, 5, 8};
    std::cout << "STL size: " << values.size() << ", last: " << values.back() << '\n';
}
