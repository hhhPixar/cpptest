// 源码模型 5：容器适配器几乎不存数据；它通过“限制底层容器接口”表达更强语义。
#include <deque>
#include <iostream>
#include <utility>

// std::stack 的简化轮廓：底层容器可替换，只要具备 back/push_back/pop_back。
template <typename T, typename Container = std::deque<T>>
class MiniStack {
public:
    // 下列操作只是把“栈术语”翻译为底层容器的尾部操作。
    // 与 std::stack 一样，对空栈调用 top()/pop() 是前置条件违例；本模型不额外检查。
    bool empty() const { return container_.empty(); }
    std::size_t size() const { return container_.size(); }
    T& top() { return container_.back(); }

    // 左值 push 复制元素；右值 push 将资源移动到容器末尾。
    void push(const T& value) { container_.push_back(value); }
    void push(T&& value) { container_.push_back(std::move(value)); }
    // pop 只删除，不返回元素：若先返回引用/值再删除，异常安全和生命周期语义会变复杂。
    void pop() { container_.pop_back(); }

private:
    Container container_; // 组合而不是继承：不向用户泄漏不该使用的底层操作。
};

int main() {
    MiniStack<int> stack;
    stack.push(10);
    stack.push(20);
    std::cout << "top: " << stack.top() << ", size: " << stack.size() << '\n';
    stack.pop();

    // 设计要点：没有 begin()、operator[]、push_front() 等接口，
    // 调用者因此不能意外破坏“只能从栈顶操作”的 LIFO 抽象。
}
