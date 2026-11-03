#include <array>
#include <iostream>

int main() {
    // std::array 是固定长度容器，兼具原生数组的连续存储和容器接口。
    std::array<int, 4> values{{10, 20, 30, 40}};
    values.at(1) = 25;  // at() 会进行边界检查；越界时抛出异常。

    std::cout << "大小：" << values.size() << '\n';
    std::cout << "首尾：" << values.front() << ", " << values.back() << '\n';
    for (int value : values) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
}
