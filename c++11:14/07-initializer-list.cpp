#include <initializer_list>
#include <iostream>

// initializer_list 让函数接收一组同类型的花括号元素。
void print(std::initializer_list<int> values) {
    for (int value : values) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
}

class P {
public:
    P(int first, int second) { std::cout << "P(int, int): " << first << ' ' << second << '\n'; }
    P(std::initializer_list<int> values) {
        std::cout << "P(initializer_list): ";
        for (int value : values) {
            std::cout << value << ' ';
        }
        std::cout << '\n';
    }
};

int main() {
    print({1, 2, 3, 4});
    P parentheses(77, 5);                 // 圆括号优先匹配 (int, int)。
    P braces{77, 5};                      // 花括号优先考虑 initializer_list 构造函数。
    P more_braces{77, 5, 23};
    (void)parentheses;
    (void)braces;
    (void)more_braces;
}
