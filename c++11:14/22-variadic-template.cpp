#include <algorithm>
#include <iostream>
#include <stdexcept>

void print_line() { std::cout << '\n'; }

template <typename T, typename... Types>
void print_line(const T& first, const Types&... rest) {
    std::cout << first << " (left: " << sizeof...(rest) << ") ";
    print_line(rest...);
}

// 递归格式化函数：%% 输出一个百分号，单个 % 消耗一个实参。
void format_print(const char* text) {
    while (*text != '\0') {
        if (*text == '%' && *++text != '%') {
            throw std::runtime_error("format string has too few arguments");
        }
        std::cout << *text++;
    }
}

template <typename T, typename... Args>
void format_print(const char* text, const T& value, const Args&... args) {
    while (*text != '\0') {
        if (*text == '%' && *++text != '%') {
            std::cout << value;
            format_print(++text, args...);
            return;
        }
        std::cout << *text++;
    }
    throw std::runtime_error("format string has too few placeholders");
}

int maximum(int value) { return value; }

template <typename... Args>
int maximum(int first, Args... rest) {
    return std::max(first, maximum(rest...));
}

int main() {
    print_line(1, 2, 3, "text");
    format_print("name=%, score=%, %%\n", "Ada", 100);
    std::cout << maximum(1, 2, 3, 4) << '\n';
}
