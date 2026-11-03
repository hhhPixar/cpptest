// 源码模型 4：分离链接哈希表的最小设计。
// 重点：bucket 数组、hash、等价比较、负载因子与 rehash 为什么会使迭代器失效。
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

template <typename Key, typename Value, typename Hash = std::hash<Key>, typename Equal = std::equal_to<Key>>
class MiniHashMap {
    // 每个桶只保存链首指针；哈希冲突的元素通过 next 串成一条链。
    // 真实实现可能用节点缓存、哨兵或不同链表布局，但“桶 + 冲突处理”的逻辑相同。
    struct Node {
        Key key;
        Value value;
        Node* next;
    };

public:
    explicit MiniHashMap(std::size_t bucket_count = 4) : buckets_(bucket_count, nullptr) {}
    MiniHashMap(const MiniHashMap&) = delete;
    MiniHashMap& operator=(const MiniHashMap&) = delete;

    ~MiniHashMap() { clear(); }

    Value* find(const Key& key) {
        // 第一步只靠 hash 定位“可能出现”的桶；同一桶并不表示键一定相同。
        Node* node = buckets_[bucket_index(key)];
        while (node) {
            // 第二步必须用 Equal 确认。正确的自定义 Hash 要满足：
            // 若 Equal(a, b) 为真，则 Hash(a) 与 Hash(b) 必须给出同一哈希值。
            if (equal_(node->key, key)) {
                return &node->value;
            }
            node = node->next; // 哈希冲突时才需要沿链继续比较。
        }
        return nullptr;
    }

    void insert_or_assign(const Key& key, const Value& value) {
        if (Value* existing = find(key)) {
            *existing = value;
            return;
        }

        // 负载因子 = 元素数 / 桶数。桶过少时链变长，平均 O(1) 可能退化。
        if (static_cast<float>(size_ + 1) / buckets_.size() > max_load_factor_) {
            rehash(buckets_.size() * 2);
        }
        const std::size_t index = bucket_index(key);
        // 头插是 O(1)：新节点的 next 接住旧链首，再把桶指向新节点。
        // 因而同桶元素的遍历顺序不是插入顺序，标准 unordered_* 也不承诺顺序。
        buckets_[index] = new Node{key, value, buckets_[index]};
        ++size_;
    }

    std::size_t size() const { return size_; }
    std::size_t bucket_count() const { return buckets_.size(); }

private:
    std::size_t bucket_index(const Key& key) const {
        return hash_(key) % buckets_.size();
    }

    void rehash(std::size_t new_bucket_count) {
        // 新桶数组先全部为空；不创建新节点，只把旧节点重新链接到不同桶。
        std::vector<Node*> new_buckets(new_bucket_count, nullptr);
        for (Node*& head : buckets_) {
            while (head) {
                Node* node = head;
                head = head->next;
                const std::size_t new_index = hash_(node->key) % new_bucket_count;
                node->next = new_buckets[new_index];
                new_buckets[new_index] = node;
            }
        }
        buckets_.swap(new_buckets);
        // 节点在本模型中未重新分配，但标准容器仍规定 rehash 会使迭代器失效；
        // 使用者不能依赖节点地址或桶布局这一实现细节。
    }

    void clear() {
        for (Node*& head : buckets_) {
            while (head) {
                Node* doomed = head;
                head = head->next;
                delete doomed;
            }
        }
    }

    std::vector<Node*> buckets_;
    std::size_t size_ = 0;
    float max_load_factor_ = 0.75F;
    Hash hash_;
    Equal equal_;
};

int main() {
    MiniHashMap<std::string, int> visits;
    visits.insert_or_assign("/home", 1);
    visits.insert_or_assign("/docs", 2);
    visits.insert_or_assign("/home", 3);

    if (const int* count = visits.find("/home")) {
        std::cout << "/home: " << *count << '\n';
    }
    std::cout << "size=" << visits.size() << ", buckets=" << visits.bucket_count() << '\n';
}
