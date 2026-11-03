#include <iostream>

// 函数模板由实参推导 T，并在需要时生成对应版本。
template <typename T>
T larger(const T& left, const T& right) {
    return left < right ? right : left;
}

int main() {
    std::cout << larger(3, 7) << '\n';
    std::cout << larger(1.5, 0.5) << '\n';
}
