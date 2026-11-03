// Effective C++（第3版）Item 16：成对使用 new 和 delete 时要采取相同形式。
// new 对应 delete；new[] 对应 delete[]。混用会导致未定义行为。
// 实际代码优先用容器和智能指针，让类型系统保存“单个/数组”的删除规则。
#include <iostream>
#include <memory>

class Token {
public:
    Token() { std::cout << "construct token\n"; }
    ~Token() { std::cout << "destroy token\n"; }
};

int main() {
    // unique_ptr<Token> 的默认删除器使用 delete，匹配 new Token。
    // 单对象与数组对象的释放形式不同，故二者的 unique_ptr 类型也不同。
    std::unique_ptr<Token> one(new Token);

    {
        // unique_ptr<Token[]> 的删除器使用 delete[]，匹配 new Token[2]。
        // [] 特化还提供下标访问，这也使“管理的是数组”在接口层面可见。
        std::unique_ptr<Token[]> many(new Token[2]);
        std::cout << "array has two tokens\n";
        // many[0]、many[1] 的析构顺序由 delete[] 负责，无需手工循环。
    } // 此处打印两次 destroy token。

    std::cout << "single token remains until main ends\n";
    // 错误示例（不要执行）：Token* array = new Token[2]; delete array;
    // 上句应使用 delete[]；用智能指针可让这种匹配关系保留在类型中。
    // 不写 delete one.get()；所有权已经交给 unique_ptr，手工释放会造成重复删除。
}
