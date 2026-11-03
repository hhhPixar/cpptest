#include <iostream>
#include <vector>

// 第二个模板参数要求传入“单个类型参数”的容器模板。
template <typename T, template <class> class Container>
class XCls {
public:
    XCls() : container_(100, T{}) {}
    std::size_t size() const { return container_.size(); }

private:
    Container<T> container_;
};

// vector 实际有两个模板参数；别名模板把 allocator 隐藏后即可匹配 XCls 的要求。
template <typename T>
using Vec = std::vector<T>;

class MyString {};

int main() {
    XCls<MyString, Vec> objects;
    std::cout << objects.size() << '\n';
}
