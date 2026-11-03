#include <iostream>
#include <vector>

// using 可以给类型取别名。与 typedef 相比，函数指针的写法更符合“名字在左边”的阅读习惯。
using BinaryOperation = int (*)(int, int);
using Scores = std::vector<int>;

int add(int left, int right) {
    return left + right;
}

int main() {
    BinaryOperation operation = add;
    Scores scores{88, 95, 100};

    std::cout << "运算结果：" << operation(3, 4) << '\n';
    std::cout << "成绩数量：" << scores.size() << '\n';
}
