#ifndef CPP_PART1_ACCOUNT_H
#define CPP_PART1_ACCOUNT_H

// static 数据成员属于类本身，而不属于某一个 Account 对象。
// 因此所有 Account 对象共享同一份利率。
class Account {
public:
    static void set_rate(double rate) { m_rate = rate; }
    static double rate() { return m_rate; }

private:
    static double m_rate;  // 这里只是声明；必须在某个 .cpp 中定义一次。
};

#endif
