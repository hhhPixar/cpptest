#include <iostream>

// 主模板适用于任意两种类型。
template <typename First, typename Second>
struct Test {
    static const char* name() { return "general Test"; }
};

// 偏特化：只固定第一个模板实参为 int，第二个仍然是模板参数。
template <typename Second>
struct Test<int, Second> {
    static const char* name() { return "Test<int, Second>"; }
};

template <typename T>
class C {
public:
    static const char* name() { return "ordinary type"; }
};

// 偏特化：匹配所有指针类型 T*。
template <typename T>
class C<T*> {
public:
    static const char* name() { return "pointer type"; }
};

int main() {
    std::cout << Test<double, char>::name() << '\n';
    std::cout << Test<int, char>::name() << '\n';
    std::cout << C<int>::name() << '\n';
    std::cout << C<int*>::name() << '\n';
}
