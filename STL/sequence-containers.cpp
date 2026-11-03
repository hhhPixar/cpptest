// 学习目标：比较顺序容器的插入位置、访问能力与迭代器失效规则。
#include <deque>
#include <forward_list>
#include <iostream>
#include <list>
#include <string>
#include <vector>

int main() {
    // vector 是连续内存，支持 O(1) 下标访问；尾部追加摊销 O(1)。
    // push_back 后 size 由 3 变 4；若发生扩容，旧迭代器、指针和引用全部失效。
    std::vector<int> numbers{1, 2, 3};
    numbers.push_back(4);
    std::cout << "vector[2] = " << numbers[2] << '\n';

    // deque 支持 O(1) 的头尾插入；它不是单块连续内存，不能用 data() 假定整体连续。
    // 两次入队后逻辑顺序为 edit、compile、test、run；两端插入也可能使迭代器失效。
    std::deque<std::string> tasks{"compile", "test"};
    tasks.push_front("edit");
    tasks.push_back("run");
    std::cout << "deque front/back = " << tasks.front() << '/' << tasks.back() << '\n';

    // list 是双向链表：已知位置插入/删除通常 O(1)，但定位和按下标访问为 O(N)，且不支持 []。
    // sort 重接链表节点而非要求随机访问迭代器；除被删除元素外，迭代器通常保持有效。
    std::list<int> linked{3, 1, 2};
    linked.sort(); // 状态变为 1, 2, 3。
    linked.push_front(0); // 状态变为 0, 1, 2, 3。

    // forward_list 是单向链表，空间更紧凑；没有 size()，只能在“某元素之后”插入。
    // first 指向 1，insert_after(first, 99) 后链为 1, 99, 2, 3；first 仍有效。
    std::forward_list<int> singly{2, 3};
    singly.push_front(1);
    const auto first = singly.begin();
    singly.insert_after(first, 99);

    std::cout << "list: ";
    for (int value : linked) {
        std::cout << value << ' ';
    }
    std::cout << "\nforward_list: ";
    for (int value : singly) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
}
