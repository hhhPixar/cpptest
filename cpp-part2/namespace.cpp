#include <iostream>

// 命名空间隔离同名标识符，避免不同库之间的命名冲突。
namespace test1 {
void echo() { std::cout << "test1::echo\n"; }
}

namespace test2 {
void echo() { std::cout << "test2::echo\n"; }
}

void echo() { std::cout << "global echo\n"; }

int main() {
    test1::echo();
    test2::echo();
    ::echo();                            // :: 表示全局命名空间。
}
