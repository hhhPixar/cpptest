#include <iostream>

// Item 27：尽量使用 C++ 风格转型，并让转型意图一目了然。
// static_cast：数值转换、明确的上行转换等受编译器检查的转换。
// const_cast：仅增减 cv 限定；dynamic_cast：多态层次中的运行时检查；
// reinterpret_cast：底层位模式解释，风险最高，应极少使用。

class Window {
public:
    virtual ~Window() = default;

    virtual void onResize() {
        width_ = 10;
        height_ = 10;
    }

protected:
    int width_ = 0;
    int height_ = 0;
};

class SpecialWindow : public Window {
public:
    void onResize() override {
        // 调用基类版本时，限定名 Window:: 仍让函数作用于当前 *this 对象。
        Window::onResize();

        // 不要写成 static_cast<Window>(*this).onResize()：该写法先构造基类临时对象，
        // 修改的是副本，不能更新当前 SpecialWindow 的基类子对象。
        std::cout << "resized to " << width_ << " x " << height_ << '\n';
    }
};

int main() {
    SpecialWindow window;
    window.onResize();

    // 这里的 static_cast 明确表达了安全、可见的数值转换意图。
    const int windowWidth = 15;
    const double scale = static_cast<double>(windowWidth) / 10;
    std::cout << "scale: " << scale << '\n';
}
