// 学习目标：理解迭代器类别、const 访问与迭代器适配器。
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

int main() {
    std::vector<int> values{1, 2, 3, 4};

    // begin/end 定义半开区间 [begin, end)；end 是越过末元素的哨兵，不能解引用。
    // 每次 *it 直接改写当前元素，循环后 values 的状态为 10, 20, 30, 40。
    for (auto it = values.begin(); it != values.end(); ++it) {
        *it *= 10; // vector::iterator 可读写元素。
    }

    // cbegin/cend 得到 const_iterator，借此表达“遍历但不修改元素”；
    // 它可解引用读取，却不能写入 *it。
    for (auto it = values.cbegin(); it != values.cend(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << '\n';

    // reverse_iterator 将递增方向反转；rbegin/rend 同样形成半开区间，输出 40, 30, 20, 10。
    std::cout << "reverse: ";
    for (auto it = values.rbegin(); it != values.rend(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << '\n';

    // list 只有双向迭代器，不能执行 it + n；不同容器支持的算法取决于迭代器类别。
    std::list<int> linked{1, 2, 3};
    auto position = linked.begin();
    // 对 list，advance 逐步移动，复杂度 O(N)；移动距离不得越过有效范围。
    std::advance(position, 2);
    std::cout << "list third: " << *position << '\n';
}
