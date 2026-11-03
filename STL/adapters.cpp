// 学习目标：容器适配器以受限接口表达 stack、queue、priority_queue 的使用意图。
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <vector>

int main() {
    // stack 默认以 deque 为底层容器，只暴露后进先出（LIFO）操作，刻意不提供迭代器。
    // 第二次 push 后 top 是 "format line"；top/pop 的前置条件均为非空，操作 O(1)。
    std::stack<std::string> undo;
    undo.push("insert text");
    undo.push("format line");
    std::cout << "undo: " << undo.top() << '\n';
    undo.pop(); // 删除当前 top，下一项恢复为 "insert text"。

    // queue 默认以 deque 为底层容器，只暴露先进先出（FIFO）操作。
    // push 101、102 后 front 是先到的 101；front/pop 同样要求非空，操作 O(1)。
    std::queue<int> waiting;
    waiting.push(101);
    waiting.push(102);
    std::cout << "serve: " << waiting.front() << '\n';
    waiting.pop(); // 101 出队，102 成为新的 front。

    // priority_queue 默认是大顶堆：top() 总给出按比较器“最大”的元素，不是完整有序序列。
    // 每次 push/pop 维护堆为 O(log N)，只查看 top 为 O(1)；它不提供按插入顺序的遍历。
    std::priority_queue<int> priorities;
    priorities.push(3);
    priorities.push(1);
    priorities.push(5);
    std::cout << "highest priority: " << priorities.top() << '\n';
}
