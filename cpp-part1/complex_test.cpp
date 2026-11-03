#include "complex.h"

#include <iostream>

int main() {
    complex first(1, 2);
    complex second(1, 2);

    // 成员函数可读取同类对象的 private 成员。
    std::cout << first.sum_parts(second) << '\n';

    first += second;
    std::cout << first << ' ' << second << '\n';

    // 两个二元 + 重载分别支持 complex + double 与 double + complex。
    first = second + 1.0;
    std::cout << first << '\n';
    first = 1.0 + second;
    std::cout << first << '\n';
    first = first + second;
    std::cout << first << '\n';
}
