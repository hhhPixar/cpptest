#include <iostream>
#include <string>
#include <utility>

// decltype 根据表达式推导类型，但不会计算该表达式。
template <typename Left, typename Right>
auto add(const Left& left, const Right& right) -> decltype(left + right) {
    return left + right;
}

int main() {
    int count = 3;
    decltype(count) another_count = 5;  // 推导为 int。

    // 对未加括号的变量名，decltype(name) 得到其声明类型；
    // 对一般左值表达式 (count)，decltype((count)) 得到 int&。
    decltype((count)) reference = count;
    reference += another_count;

    auto message = add(std::string("总数："), std::to_string(count));
    std::cout << message << '\n';
}
