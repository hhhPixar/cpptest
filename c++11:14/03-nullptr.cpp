#include <iostream>
#include <cstddef>

void select(int) {
    std::cout << "调用 select(int)\n";
}

void select(void*) {
    std::cout << "调用 select(void*)\n";
}

int main() {
    // 0 是整数，因此会匹配整数重载；nullptr 则是专门的空指针字面量。
    select(0);
    select(nullptr);

    std::nullptr_t empty = nullptr;
    if (empty == nullptr) {
        std::cout << "empty 是空指针值\n";
    }
}
