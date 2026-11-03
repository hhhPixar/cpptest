#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    // lambda 会生成一个匿名函数对象；auto 保存这个闭包对象的实际类型。
    auto greeting = [] {
        std::cout << "hello lambda\n";
    };
    greeting();

    std::vector<int> values{1, 2, 31, 4, 50};
    int lower = 1;
    int upper = 8;

    // [lower, upper] 是按值捕获：lambda 保存两个局部变量的副本。
    // remove_if 只把“不删除”的元素移到前面，并不改变 vector 的 size。
    const auto new_end = std::remove_if(
        values.begin(), values.end(),
        [lower, upper](int value) { return lower < value && value < upper; });

    // erase 才真正删除 [new_end, end()) 中的“逻辑上被移除”的元素。
    // 这套惯用法称为 erase-remove idiom。
    values.erase(new_end, values.end());

    for (int value : values) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
}
