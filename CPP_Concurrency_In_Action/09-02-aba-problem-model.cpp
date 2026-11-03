#include <iostream>
#include <string>

// 这不是并发算法，而是确定性状态机：只展示裸指针 CAS 为什么看不见 ABA。
struct head_state {
    char address;       // 用 A/B/C 代表地址，避免分配或释放真实节点。
    unsigned version;   // 每次逻辑修改都递增的标签。
};

bool equal_address(const head_state& left, const head_state& right) {
    return left.address == right.address;
}

bool equal_tagged(const head_state& left, const head_state& right) {
    return left.address == right.address && left.version == right.version;
}

int main() {
    // T1 读到 A(v7)，准备执行“若仍是 A 则改为 B”的 CAS。
    const head_state t1_expected{'A', 7};
    head_state head = t1_expected;

    // T2 把 A 取走：head 变 B(v8)；随后复用逻辑位置 A：head 又是 A(v9)。
    // 地址恢复原值，但它已不是 T1 当初观察到的那个状态。
    head = {'B', 8};
    head = {'A', 9};

    std::cout << "T1 期望: " << t1_expected.address << "(v"
              << t1_expected.version << "), 当前: " << head.address << "(v"
              << head.version << ")\n";
    std::cout << "仅比较地址的 CAS 会" << (equal_address(t1_expected, head) ? "错误接受" : "拒绝")
              << "\n";
    std::cout << "地址加标签的 CAS 会" << (equal_tagged(t1_expected, head) ? "接受" : "拒绝")
              << "\n";

    // 标签只说明“状态变过”，不是完整回收方案：实际工程还必须处理标签溢出、
    // 宽 CAS 可用性，以及所有读者在旧节点销毁前完成访问等问题。
    return !equal_address(t1_expected, head) || !equal_tagged(t1_expected, head) ? 0 : 1;
}
