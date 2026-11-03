#include <iostream>

// 最小“指针式类”示例：operator* 和 operator-> 让对象拥有指针语法。
// 它不拥有资源，调用者必须保证传入指针在使用期间有效。
template <class T>
class PointerView {
public:
    explicit PointerView(T* pointer) : pointer_(pointer) {}
    T& operator*() const { return *pointer_; }
    T* operator->() const { return pointer_; }

private:
    T* pointer_;
};

struct Point {
    int x;
    void move_right() { ++x; }
};

int main() {
    Point point{1};
    PointerView<Point> view(&point);
    view->move_right();
    std::cout << (*view).x << '\n';
}
