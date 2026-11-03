#include <iostream>
#include <map>
#include <string>

int main() {
    // auto 根据初始化表达式推导变量类型，适合书写较长的迭代器类型。
    auto count = 42;              // 推导为 int。
    auto ratio = 3.14;            // 推导为 double。
    const auto title = "C++14";  // const 同样会保留在推导结果中。

    std::map<std::string, int> scores{{"Ada", 100}, {"Bob", 88}};
    for (auto it = scores.begin(); it != scores.end(); ++it) {
        std::cout << it->first << ": " << it->second << '\n';
    }

    std::cout << count << ", " << ratio << ", " << title << '\n';
}
