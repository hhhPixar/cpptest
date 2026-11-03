// 源码模型 6：从普通二叉搜索树理解 map/set 的“比较器决定顺序”。
// std::map/std::set 通常采用红黑树等平衡树；本模型省略旋转与颜色，只保留查找路径。
#include <functional>
#include <iostream>
#include <string>
#include <utility>

template <typename Key, typename Value, typename Compare = std::less<Key>>
class MiniOrderedMap {
    // BST 不变量：left 子树中每个键都“小于”本键，right 子树中每个键都“大于”本键。
    // 这里的小于由 Compare 定义，而不是写死 operator<。
    struct Node {
        Key key;
        Value value;
        Node* left = nullptr;
        Node* right = nullptr;
        Node(const Key& input_key, const Value& input_value) : key(input_key), value(input_value) {}
    };

public:
    MiniOrderedMap() = default;
    MiniOrderedMap(const MiniOrderedMap&) = delete;
    MiniOrderedMap& operator=(const MiniOrderedMap&) = delete;
    ~MiniOrderedMap() { destroy(root_); }

    void insert_or_assign(const Key& key, const Value& value) {
        // link 始终指向“应当放置当前节点指针的那个槽位”：开始是 root_，
        // 向下时变成父节点的 left 或 right。使用 Node** 便于在空槽处直接写入新节点。
        Node** link = &root_;
        while (*link) {
            if (compare_(key, (*link)->key)) {
                // key 应位于当前节点左侧，继续检查左子树。
                link = &(*link)->left;
            } else if (compare_((*link)->key, key)) {
                // 当前键应位于 key 左侧，继续检查右子树。
                link = &(*link)->right;
            } else {
                // “等价”由比较器定义：!(a<b) && !(b<a)，不一定等同于 operator==。
                (*link)->value = value;
                return;
            }
        }
        // 找到空槽：此时插入叶子节点不会破坏从根到该位置的排序关系。
        *link = new Node(key, value);
    }

    Value* find(const Key& key) {
        Node* node = root_;
        while (node) {
            if (compare_(key, node->key)) {
                node = node->left;
            } else if (compare_(node->key, key)) {
                node = node->right;
            } else {
                return &node->value;
            }
        }
        return nullptr;
    }

private:
    static void destroy(Node* node) {
        if (!node) {
            return;
        }
        // 后序遍历：先释放子树，再释放拥有子树链接的当前节点。
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    Node* root_ = nullptr;
    Compare compare_;
};

int main() {
    MiniOrderedMap<int, std::string> names;
    names.insert_or_assign(20, "twenty");
    names.insert_or_assign(10, "ten");
    names.insert_or_assign(30, "thirty");

    if (const std::string* name = names.find(10)) {
        std::cout << "10 -> " << *name << '\n';
    }

    // 不平衡 BST 在有序插入时会退化为链表 O(N)。
    // 标准 map/set 选择平衡树，正是为了把查找、插入、删除稳定在 O(log N)。
}
