#include <iostream>
#include <vector>

int main() {
    std::vector<int> values{1, 2, 3};

    // auto 从初始化表达式推导类型，省去冗长的 std::vector<int>::iterator。
    auto it = values.begin();
    std::cout << *it << '\n';

    // auto 必须有初始化式，编译器才能完成类型推导。
    // auto uninitialized;  // 错误：无法推导类型。
}
