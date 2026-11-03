#include <iostream>

// 第 4 章实验：inline 的首要含义是 ODR（单一定义规则）许可，
// 允许同一个 inline 定义出现在多个翻译单元；它不是“必须内联展开”的命令。
inline int clampToByte(int value) {
    return value < 0 ? 0 : (value > 255 ? 255 : value);
}

class Pixel {
public:
    explicit Pixel(int intensity) : intensity_(clampToByte(intensity)) {}

    // 类内定义的成员函数隐式 inline；优化器可自行选择是否真正展开调用。
    int intensity() const { return intensity_; }

    void brighten(int amount) {
        intensity_ = clampToByte(intensity_ + amount);
    }

private:
    int intensity_;
};

int main() {
    Pixel pixel(250);
    pixel.brighten(20);

    // 函数地址可用于证明“inline”不妨碍函数作为可调用实体存在；
    // 但输出的地址本身不具可移植的教学意义，故不打印它。
    int (*clampFunction)(int) = &clampToByte;
    std::cout << "clampToByte(-3) = " << clampFunction(-3) << '\n';
    std::cout << "clamped intensity = " << pixel.intensity() << '\n';
    // 即使以 -O0 编译而保留真实调用，程序语义也完全相同。
}
