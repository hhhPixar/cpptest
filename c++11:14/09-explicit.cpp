#include <iostream>

class Distance {
public:
    // C++11 起，多个参数的构造函数也可以 explicit，阻止隐式转换。
    explicit Distance(int meters, int centimeters = 0)
        : centimeters_(meters * 100 + centimeters) {}

    int centimeters() const { return centimeters_; }

private:
    int centimeters_;
};

void print_distance(const Distance& distance) {
    std::cout << distance.centimeters() << " 厘米\n";
}

int main() {
    Distance walk{3, 25};
    print_distance(walk);

    // print_distance({3, 25});  // 错误：explicit 构造函数不参与隐式转换。
    print_distance(Distance{1, 50});  // 需要时显式构造对象。
}
