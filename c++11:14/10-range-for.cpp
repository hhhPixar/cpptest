#include <iostream>
#include <vector>

int main() {
    std::vector<int> values{1, 2, 3, 4};

    // 按值遍历适合只读；此处修改的是 value 的副本。
    for (auto value : values) {
        std::cout << value << ' ';
    }
    std::cout << '\n';

    // 使用引用可直接修改容器中的元素。
    for (auto& value : values) {
        value *= 10;
    }

    // const 引用避免复制，也明确表示不会修改元素。
    for (const auto& value : values) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
}
