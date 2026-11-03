#include <iostream>

// Item 2：优先让编译器看见常量，而不是用预处理器替换字面量。
// 宏没有作用域和类型信息，诊断信息中往往只剩下展开后的数字。
#define ASPECT_RATIO_MACRO 1.653
constexpr double kAspectRatio = 1.653;

class GamePlayer {
public:
    static int numTurns() {
        return kNumTurns;
    }

private:
    // 整数型 static constexpr 成员可直接在类内初始化。
    // 用它作数组长度等编译期常量时也具有清晰的类作用域。
    static constexpr int kNumTurns = 5;
};

// C++11/14 中，当 constexpr 静态成员被 ODR-use 时仍需类外定义一次；
// C++17 起 inline static data member 规则可免除此定义。
constexpr int GamePlayer::kNumTurns;

class CostEstimate {
public:
    static double fudgeFactor() {
        return kFudgeFactor;
    }

private:
    // 非整型常量同样可由 constexpr 表达“编译期常量”的意图。
    static constexpr double kFudgeFactor = 0.0;
};

constexpr double CostEstimate::kFudgeFactor;

int main() {
    std::cout << "macro ratio: " << ASPECT_RATIO_MACRO << '\n';
    std::cout << "typed ratio: " << kAspectRatio << '\n';
    std::cout << "turns: " << GamePlayer::numTurns() << '\n';
    std::cout << "fudge factor: " << CostEstimate::fudgeFactor() << '\n';
}
