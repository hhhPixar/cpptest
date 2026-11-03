#include <iostream>
#include <vector>

// 别名模板把“带参数的类型别名”封装成可复用名称。
template <typename T>
using Vector = std::vector<T>;

template <typename T>
using Pointer = T*;

int main() {
    Vector<int> values{1, 2, 3};
    Pointer<int> first = &values.front();
    std::cout << *first << '\n';
}
