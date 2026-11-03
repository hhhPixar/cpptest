#include <iostream>
#include <list>
#include <vector>

int main() {
    // C++11 前，连续的 >> 会被解析为右移运算符，需写成 > >。
    // C++11 起，编译器能在模板上下文中正确识别嵌套模板的 >>。
    std::vector<std::list<int>> groups{{1, 2}, {3, 4, 5}};

    std::cout << "组数：" << groups.size() << '\n';
    std::cout << "第二组元素数：" << groups[1].size() << '\n';
}
