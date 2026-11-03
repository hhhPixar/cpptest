#include <iostream>

// 主模板是通用实现。
template <class T>
struct MyHash {
    T operator()(T value) const { return value; }
};

// 全特化为指定类型提供不同实现；这里使用不同常量演示确实选中了特化。
template <>
struct MyHash<char> {
    int operator()(char value) const { return static_cast<unsigned char>(value); }
};

template <>
struct MyHash<long> {
    long operator()(long value) const { return value % 97; }
};

int main() {
    std::cout << MyHash<long>{}(1000) << '\n';
    std::cout << MyHash<char>{}('a') << '\n';
    std::cout << MyHash<int>{}(10) << '\n'; // int 使用主模板。
}
