#include <iostream>
#include <string>

// Item 38：通过复合塑模出 has-a 或“根据某物实现出”。
// Person 有一个名字，AddressBook 有一个 Person 成员，表达“通讯录拥有联系人”。
// private 继承也能复用实现，但语义不同；这里选择复合以保留正确的 has-a 关系。
class Person {
public:
    explicit Person(std::string name) : name_(std::move(name)) {}
    void introduce() const { std::cout << "联系人：" << name_ << '\n'; }
private:
    std::string name_;
};

class AddressBook {
public:
    explicit AddressBook(std::string owner) : owner_(std::move(owner)) {}
    void showOwner() const {
        std::cout << "通讯录中的";
        owner_.introduce();
    }
private:
    Person owner_; // AddressBook has-a Person；生命周期由成员自动管理。
};

int main() {
    AddressBook book("Ada");
    book.showOwner();
    // 客户端不能把 AddressBook 当作 Person，这是复合的有意限制。
}
