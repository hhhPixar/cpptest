#include <iostream>

// 参数包（parameter pack）可以在模板中接收任意数量、任意类型的实参。
void print_all() {
    std::cout << '\n';  // 递归终止条件：没有参数时只换行。
}

template <typename T, typename... Rest>
void print_all(const T& first, const Rest&... rest) {
    std::cout << first;
    if (sizeof...(rest) != 0) {
        std::cout << ", ";
    }
    print_all(rest...);  // 展开其余实参，直到参数包为空。
}

template <typename... Types>
void show_count(const Types&...) {
    // sizeof... 可在编译期取得参数包中的参数数量。
    std::cout << "参数个数：" << sizeof...(Types) << '\n';
}

int main() {
    print_all("编号", 7, "分数", 95.5);
    show_count("编号", 7, "分数", 95.5);
}
