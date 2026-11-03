#include <iostream>

// 成员模板允许由“可转换的元素类型”构造另一种 Pair。
template <class First, class Second>
struct Pair {
    First first;
    Second second;

    Pair() : first(), second() {}
    Pair(const First& first_value, const Second& second_value)
        : first(first_value), second(second_value) {}

    template <class OtherFirst, class OtherSecond>
    Pair(const Pair<OtherFirst, OtherSecond>& other)
        : first(other.first), second(other.second) {}
};

class Base1 {};
class Fish : public Base1 {};
class Base2 {};
class Duck : public Base2 {};

int main() {
    Pair<Fish, Duck> animals;
    Pair<Base1, Base2> bases(animals);    // Fish/Duck 分别向上转换为基类对象。
    std::cout << sizeof(bases) << '\n';
}
