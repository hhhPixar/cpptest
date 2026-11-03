#include <iostream>
#include <string>
#include <unordered_map>

int main() {
    // unordered_map 是基于哈希表的键值容器：哈希函数先把 key 映射到桶（bucket），
    // 再在桶内比较键以处理哈希冲突。查找、插入、删除的平均复杂度为 O(1)。
    std::unordered_map<std::string, int> inventory{
        {"apple", 12},
        {"banana", 8},
        {"orange", 15},
    };

    // operator[] 在键不存在时会插入默认值；已存在时可直接更新对应 value。
    inventory["banana"] += 2;

    // find 不会插入元素，适合只读查询。
    const auto found = inventory.find("banana");
    if (found != inventory.end()) {
        std::cout << found->first << " 的库存：" << found->second << '\n';
    }

    // 桶数和装载因子影响冲突概率；rehash 可以预留更多桶以减少扩容和冲突。
    inventory.rehash(16);
    std::cout << "元素数：" << inventory.size()
              << ", 桶数：" << inventory.bucket_count() << '\n';
}
