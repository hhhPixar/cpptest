#include <iostream>
#include <string>

// 第 5 章实验：派生类构造总是先构造基类；复制派生类时也先复制基类子对象。
// 输出顺序用于观察语言保证的生命周期顺序，而非依赖任何特定对象内存布局。
class Named {
public:
    explicit Named(std::string name) : name_(std::move(name)) {
        std::cout << "Named ctor: " << name_ << '\n';
    }

    Named(const Named& other) : name_(other.name_) {
        std::cout << "Named copy: " << name_ << '\n';
    }

    ~Named() {
        std::cout << "Named dtor: " << name_ << '\n';
    }

protected:
    std::string name_;
};

class Employee : public Named {
public:
    Employee(std::string name, int id) : Named(std::move(name)), id_(id) {
        std::cout << "Employee ctor: " << id_ << '\n';
    }

    Employee(const Employee& other) : Named(other), id_(other.id_) {
        std::cout << "Employee copy: " << id_ << '\n';
    }

    ~Employee() {
        std::cout << "Employee dtor: " << id_ << '\n';
    }

private:
    int id_;
};

int main() {
    std::cout << "--- construct ---\n";
    Employee original("Ada", 42);
    std::cout << "--- copy ---\n";
    Employee copy = original;
    std::cout << "--- scope exit: derived then base ---\n";
}
