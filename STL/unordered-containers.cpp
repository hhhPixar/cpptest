// 学习目标：无序关联容器基于哈希表；平均 O(1) 查找，不保证遍历顺序。
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

int main() {
    // unordered_set 只保留唯一键。输出顺序由哈希桶布局决定，不要依赖它。
    // insert/count 平均 O(1)、碰撞严重时最坏 O(N)；重复的 "cpp" 不会形成第二项。
    std::unordered_set<std::string> tags{"cpp", "stl", "cpp"};
    tags.insert("algorithm");
    std::cout << "contains stl: " << (tags.count("stl") != 0) << '\n';

    // unordered_map 是键到值的哈希映射；operator[] 在不存在键时先插入值初始化的 int。
    // 因而三次递增后 /home 为 2、/docs 为 1；若只查询请用 find，避免意外插入。
    std::unordered_map<std::string, int> visits;
    ++visits["/home"];
    ++visits["/docs"];
    ++visits["/home"];

    const auto home = visits.find("/home"); // 未命中会返回 end，解引用前必须判断。
    if (home != visits.end()) {
        std::cout << home->first << ": " << home->second << '\n';
    }

    // 负载因子过高时插入可触发 rehash；rehash 会使所有迭代器失效，
    // 但指向元素的引用/指针通常仍有效。需要长期保存迭代器时，应在插入后重新取得。
    std::cout << "bucket_count: " << visits.bucket_count()
              << ", load_factor: " << visits.load_factor() << '\n';
}
