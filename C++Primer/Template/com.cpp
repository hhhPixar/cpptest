#include <iostream>

// 函数模板把“比较规则”抽象为类型参数 T：只要 T 支持 < 和 >，
// 编译器便可在调用点生成对应的 compare<T> 实例。
template <typename T>
int compare(const T& left, const T& right) {
    // 此处约定：返回负数表示 left 更大，正数表示 right 更大。
    // 这与 std::strcmp 的符号习惯相反，阅读调用方时应特别留意。
    if (left > right) {
        return -1;
    }
    if (left < right) {
        return 1;
    }
    return 0;
}

int main() {
    // 显式给出 <int> 用来观察模板实参；本例也可由实参 1、2 自动推导。
    std::cout << "compare(1, 2) = " << compare<int>(1, 2) << '\n';
}
