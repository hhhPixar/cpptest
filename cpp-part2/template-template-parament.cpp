#include <iostream>
#include <vector>

// Container 是“接受一个类型参数的类模板”，而不是某个容器对象。
template <typename T, template <typename> class Container>
class XCls {
public:
    XCls() : values_(3, T{}) {}
    std::size_t size() const { return values_.size(); }

private:
    Container<T> values_;
};

int main() {
    XCls<int, std::vector> values;
    std::cout << values.size() << '\n';
}
