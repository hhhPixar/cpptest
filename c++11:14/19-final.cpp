#include <iostream>

class Animal {
public:
    virtual void speak() const final {
        std::cout << "动物发出声音\n";
    }
};

class Dog final : public Animal {
public:
    void run() const {
        std::cout << "狗在奔跑\n";
    }

    // void speak() const override {} // 错误：Animal::speak 已标记 final，不能再重写。
};

// class Puppy : public Dog {}; // 错误：Dog 是 final 类，不能被继承。

int main() {
    Dog dog;
    dog.speak();  // 可以继承并使用 final 虚函数，只是不能重写它。
    dog.run();
}
