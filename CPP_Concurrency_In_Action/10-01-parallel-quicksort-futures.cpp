#include <algorithm>
#include <future>
#include <iostream>
#include <vector>

// 教学版并行快速排序：只在足够大的区间派生一个异步分支。
// depth 是硬性上限，避免极端划分时递归创建过多任务。
void parallel_quicksort(std::vector<int>::iterator first,
                        std::vector<int>::iterator last, unsigned depth) {
    const auto size = last - first;
    const auto sequential_limit = 32;
    if (size <= sequential_limit) {
        std::sort(first, last);
        return;
    }

    // 中点元素作枢轴；partition 后枢轴被放到最终位置。
    auto pivot = first + size / 2;
    std::iter_swap(pivot, last - 1);
    auto middle = std::partition(first, last - 1,
                                 [value = *(last - 1)](int item) { return item < value; });
    std::iter_swap(middle, last - 1);

    // 只有剩余深度时才异步；另一半由当前线程完成，任务数量有上界。
    if (depth != 0) {
        auto left = std::async(std::launch::async, [first, middle, depth] {
            parallel_quicksort(first, middle, depth - 1);
        });
        parallel_quicksort(middle + 1, last, depth - 1);
        left.get(); // 等待并传播子任务异常。
    } else {
        std::sort(first, middle);
        std::sort(middle + 1, last);
    }
}

int main() {
    // 固定且很小的输入是本例的资源限制之一，不把递归策略用于无界数据。
    std::vector<int> values{9, 1, 8, 2, 7, 3, 6, 4, 5, 0, 12, 10, 11, -1, 13};
    parallel_quicksort(values.begin(), values.end(), 3);

    std::cout << "sorted:";
    for (int value : values) std::cout << ' ' << value;
    std::cout << "\nvalid=" << std::is_sorted(values.begin(), values.end()) << '\n';
}
