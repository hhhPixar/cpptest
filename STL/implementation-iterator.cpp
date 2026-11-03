// 源码模型 2：迭代器标签分发。
// 同名算法 advance() 依据迭代器能力选择不同实现，而不是为每个容器复制一套算法。
#include <cstddef>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

// 标准库内部会使用 iterator_traits 从迭代器类型提取 category、value_type 等关联类型。
template <typename Iterator>
void advance_impl(Iterator& it, std::ptrdiff_t distance, std::input_iterator_tag) {
    // 输入迭代器只能单向 ++；对于这种类别，不应传入负距离。
    while (distance-- > 0) {
        ++it;
    }
}

template <typename Iterator>
void advance_impl(Iterator& it, std::ptrdiff_t distance, std::bidirectional_iterator_tag) {
    // 双向迭代器（如 list）可 ++ 或 --，但不能直接跳跃 n 格。
    while (distance > 0) {
        ++it;
        --distance;
    }
    while (distance < 0) {
        --it;
        ++distance;
    }
}

template <typename Iterator>
void advance_impl(Iterator& it, std::ptrdiff_t distance, std::random_access_iterator_tag) {
    // 随机访问迭代器（如 vector 指针）支持 O(1) 跳跃。
    it += distance;
}

template <typename Iterator>
void mini_advance(Iterator& it, std::ptrdiff_t distance) {
    // iterator_traits 对原生指针也有效；例如 int* 的 category 是 random_access_iterator_tag。
    using Category = typename std::iterator_traits<Iterator>::iterator_category;
    // Category{} 只是一个无状态“类型标签”，不承载运行时数据。
    // 编译期重载决议完成分发：运行时没有 if/switch 开销。
    advance_impl(it, distance, Category{});
}

int main() {
    std::vector<int> contiguous{10, 20, 30, 40};
    auto vector_it = contiguous.begin(); // 指向 10。
    mini_advance(vector_it, 3); // 选择 random_access 版本，一步执行 it += 3，落在 40。
    std::cout << "vector result: " << *vector_it << '\n';

    std::list<int> linked{10, 20, 30, 40};
    auto list_it = linked.begin(); // 指向第一个节点 10。
    mini_advance(list_it, 2); // 选择 bidirectional 版本，实际连续执行两次 ++，落在 30。
    std::cout << "list result: " << *list_it << '\n';

    // 设计收益：算法只依赖“能力契约”，容器只提供相应迭代器，二者得以正交组合。
}
