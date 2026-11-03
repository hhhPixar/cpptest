#include <iostream>

// 递归终点：参数包耗尽后结束输出。
void print() { std::cout << '\n'; }

// 每次取出第一个实参，余下实参构成新的参数包并递归调用。
template <typename T, typename... Types>
void print(const T& first, const Types&... rest) {
    std::cout << first << " (left: " << sizeof...(rest) << ") ";
    print(rest...);
}

int main() {
    print(1, 2, 'a', 1.0);
}
