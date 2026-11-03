#include <iostream>
#include <string>

// Effective C++（第 3 版）Item 5：了解 C++ 默默编写并调用哪些函数。
// 若类没有自行声明相应特殊成员，编译器会按需要隐式声明/定义：
// 默认构造、复制构造、复制赋值、析构（现代 C++ 还涉及移动成员）。
// 它们通常执行“逐个基类子对象、逐个成员”的默认操作；这只在该语义正确时才够用。

class CompilerGenerated {
public:
    // 成员默认初始化使隐式默认构造后的 name_ 和 score_ 都有确定状态。
    std::string name_ = "anonymous";
    int score_ = 0;

    void print(const char* label) const {
        std::cout << label << ": " << name_ << ", score=" << score_ << '\n';
    }
    // 没有显式声明构造、复制构造、operator= 或析构函数。
};

int main() {
    CompilerGenerated original; // 调用编译器生成的默认构造：成员各自按默认初始化构造。
    original.name_ = "Ada";
    original.score_ = 100;

    // 隐式复制构造：std::string 会复制其值，int 也被复制；两个对象各自拥有独立 string 状态。
    CompilerGenerated copied(original);
    copied.name_ = "Grace";

    // 隐式复制赋值：将 assigned 的每个成员赋为 original 的对应成员。
    CompilerGenerated assigned;
    assigned = original;

    original.print("original");
    copied.print("copied");
    assigned.print("assigned");

    // 离开作用域时，隐式析构会按成员逆序析构：先 score_（无动作），再 name_（释放其内部资源）。
    // 若类改为持有裸 owning 指针，逐成员复制会变成浅拷贝，此时就必须自行定义复制策略或使用 RAII 成员。
}
