// 学习目标：有序关联容器以比较器排序，并在对数复杂度下按键查找。
#include <iostream>
#include <map>
#include <set>
#include <string>

int main() {
    // set 保存唯一键，默认按 std::less<Key> 升序排列；构造时重复的 80 被合并。
    // insert/find 均为 O(log N)，插入不使既有迭代器失效（被擦除的元素除外）。
    std::set<int> unique_scores{80, 60, 80, 100};
    unique_scores.insert(90); // 状态为 60, 80, 90, 100。

    const auto score = unique_scores.find(90);
    if (score != unique_scores.end()) { // find 未命中返回 end，不能解引用。
        std::cout << "found score: " << *score << '\n';
    }

    // multiset 允许等价键重复；count 为 O(log N + 匹配数)，适合需要保留所有同值记录的场景。
    std::multiset<int> repeated{1, 1, 2};
    std::cout << "number of 1: " << repeated.count(1) << '\n';

    // map 的元素类型是 pair<const Key, T>：键不可通过迭代器修改，以维持树的排序不变量。
    std::map<std::string, int> inventory{{"apple", 3}, {"banana", 5}};
    // [] 查找/插入为 O(log N)；键存在时修改原值，此处 apple 从 3 变为 5。
    // 仅查询未知键时应避免 []，否则会产生一个值初始化的条目。
    inventory["apple"] += 2;

    // find 不插入新元素，O(log N)，因而适合只读探测。
    const auto item = inventory.find("banana");
    if (item != inventory.end()) {
        std::cout << item->first << ": " << item->second << '\n';
    }

    // multimap 允许一个键映射多个值；equal_range 给出该键对应的半开区间。
    // 取得 range 后不修改 courses；若插入/擦除相关元素，应重新获取范围以表达当前状态。
    std::multimap<std::string, std::string> courses;
    courses.emplace("C++", "Alice");
    courses.emplace("C++", "Bob");
    const auto range = courses.equal_range("C++");
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->first << " student: " << it->second << '\n';
    }
}
