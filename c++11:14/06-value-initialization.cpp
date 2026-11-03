#include <iostream>

struct Record {
    int id;
    double score;
};

int main() {
    int number{};       // 值初始化：内置类型得到 0。
    int* pointer{};     // 值初始化：指针得到 nullptr。
    Record record{};    // 聚合对象的每个成员都被零初始化。

    std::cout << "number=" << number << '\n';
    std::cout << "pointer 是否为空：" << (pointer == nullptr) << '\n';
    std::cout << "record=" << record.id << ", " << record.score << '\n';
}
