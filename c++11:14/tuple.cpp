#include <iostream>
#include <string>
#include <tuple>

int main() {
    // tuple 可在一个对象中保存不同类型的固定数量元素。
    std::tuple<int, std::string, double> record{1, "C++", 9.5};
    std::cout << std::get<0>(record) << ' ' << std::get<1>(record) << ' '
              << std::get<2>(record) << '\n';
}
