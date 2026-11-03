// 源码模型 1：用不足 200 行的 MiniVector 观察 std::vector 的核心设计。
// 重点不是替代标准库，而是理解三个内部指针、容量增长、placement new 与异常安全。
#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>

// 真实 std::vector 由实现决定具体布局；常见实现可抽象为 [begin, end, capacity_end)。
// - begin_：已分配原始内存的起点；
// - end_：最后一个已构造元素的后一位；
// - capacity_end_：原始内存的终点。
template <typename T>
class MiniVector {
public:
    MiniVector() = default;
    MiniVector(const MiniVector&) = delete;             // 为突出扩容逻辑，本模型不实现复制。
    MiniVector& operator=(const MiniVector&) = delete;

    ~MiniVector() {
        destroy_and_deallocate();
    }

    // 指针相减得到两个位置之间已经构造/可容纳的元素数。
    // 空 vector 的三个指针都为 nullptr；本模型依赖“同为 nullptr 时相减结果为 0”的常见实现行为，
    // 真实标准库会以更严谨的内部表示规避这种教学模型的边界细节。
    std::size_t size() const { return begin_ ? static_cast<std::size_t>(end_ - begin_) : 0; }
    std::size_t capacity() const {
        return begin_ ? static_cast<std::size_t>(capacity_end_ - begin_) : 0;
    }

    // 与 std::vector::operator[] 一样：不做范围检查；越界是调用者错误。
    // 若要教学/生产中的受检访问，应额外实现 at() 并在越界时抛出异常。
    const T& operator[](std::size_t index) const { return begin_[index]; }

    template <typename... Args>
    void emplace_back(Args&&... args) {
        if (end_ != capacity_end_) {
            // 此时 [end_, capacity_end_) 至少有一个空槽：不搬迁任何旧元素，直接原地构造。
            // allocator_traits::construct 近似表达标准库的“在原始内存上开始对象生命周期”。
            std::allocator_traits<Allocator>::construct(allocator_, end_, std::forward<Args>(args)...);
            // 只有构造成功才推进 end_；若构造函数抛异常，size() 自动保持原值。
            ++end_;
            return;
        }
        // 没有空槽才扩容；这正是 vector 尾插偶尔为 O(N) 的时刻。
        grow_and_emplace(std::forward<Args>(args)...);
    }

private:
    using Allocator = std::allocator<T>;

    template <typename... Args>
    void grow_and_emplace(Args&&... args) {
        const std::size_t old_size = size();
        // 几何增长把 n 次尾插的总搬迁量摊销为 O(n)，所以单次 push_back 的均摊复杂度为 O(1)。
        const std::size_t new_capacity = old_size == 0 ? 1 : old_size * 2;
        // allocate 得到的是“足以放下 T 的字节区域”，其中还没有一个 T 对象。
        T* new_begin = std::allocator_traits<Allocator>::allocate(allocator_, new_capacity);
        // new_end 始终指向新区域中“下一个尚未构造的位置”，所以它也记录回滚边界。
        T* new_end = new_begin;

        try {
            // 先在新内存构造原元素：move_if_noexcept 在“移动可能抛异常且可复制”时选择复制，
            // 尽量维持扩容失败后旧 vector 不变的强异常安全保证。
            for (T* source = begin_; source != end_; ++source, ++new_end) {
                std::allocator_traits<Allocator>::construct(
                    allocator_, new_end, std::move_if_noexcept(*source));
            }
            std::allocator_traits<Allocator>::construct(allocator_, new_end, std::forward<Args>(args)...);
            ++new_end;
        } catch (...) {
            // 只销毁本次已经成功构造的对象，释放新原始内存；旧存储尚未动，仍然有效。
            destroy_range(new_begin, new_end);
            std::allocator_traits<Allocator>::deallocate(allocator_, new_begin, new_capacity);
            throw;
        }

        // 所有新对象成功后才提交：销毁旧对象，归还旧内存，再切换三根指针。
        destroy_and_deallocate();
        begin_ = new_begin;
        end_ = new_end;
        capacity_end_ = new_begin + new_capacity;
    }

    void destroy_range(T* first, T* last) {
        // 反向销毁与数组对象的析构顺序一致；对 int 这样的平凡类型它没有可见效果，
        // 对持有资源的 T 则会逐个调用析构函数，释放各元素自己的资源。
        while (last != first) {
            --last;
            std::allocator_traits<Allocator>::destroy(allocator_, last);
        }
    }

    void destroy_and_deallocate() {
        if (!begin_) {
            return;
        }
        const std::size_t old_capacity = capacity();
        destroy_range(begin_, end_);
        std::allocator_traits<Allocator>::deallocate(allocator_, begin_, old_capacity);
        begin_ = end_ = capacity_end_ = nullptr;
    }

    Allocator allocator_;
    T* begin_ = nullptr;
    T* end_ = nullptr;
    T* capacity_end_ = nullptr;
};

int main() {
    MiniVector<int> values; // 初始时 size=0、capacity=0，第一次插入必然走扩容路径。
    for (int value = 1; value <= 5; ++value) {
        values.emplace_back(value);
        std::cout << "push " << value << ": size=" << values.size()
                  << ", capacity=" << values.capacity() << '\n';
    }
    std::cout << "third element: " << values[2] << '\n';

    // 结论：一旦扩容，旧连续存储被释放，指向旧元素的指针/引用/迭代器不能再使用。
}
