#include <iostream>
#include <vector>

// Item 42：了解 typename 的双重意义。
// 在模板参数列表中，class 与 typename 同义；这里用 typename 声明类型参数。
// 在依赖名称前，typename 告诉编译器 nested_type 是类型而非静态成员。
template<typename Container>
void showFirstType(const Container& values) {
    typename Container::const_iterator it = values.begin();
    if (it != values.end()) std::cout << "首元素：" << *it << '\n';
}

int main() {
    std::vector<int> values{4, 8, 15};
    showFirstType(values);
    // 少写第二个 typename 时，解析器不能预先知道 Container::const_iterator 的种类。
}
// 依赖名称的含义要等到模板以具体 Container 实例化后才确定。
// 因此解析阶段需要 typename 消除“它可能是值”的歧义。
// 但基类列表和成员初始化列表等少数语境不应使用 typename。
// 这里使用 const_iterator，确保函数不会经由迭代器修改容器。
// vector 的迭代器类别和具体表示都被封装在库实现中。
// 只依赖约定的嵌套类型，能让函数适用于多种标准容器。
// 编译结果证明编译器正确识别了该依赖类型。
// 这个规则看似语法细节，却是泛型代码常见的诊断来源。
// 学会识别“依赖于模板参数的限定名”即可稳定处理它。
// 输出仅取首元素，避免把示例扩展成无关的遍历算法。
// Item 42 的重点始终是类型名称的解析，而非容器算法。
// 保留最小可运行上下文，便于单独编译观察。
// 每个 term 文件都拥有独立 main，故无链接依赖。
