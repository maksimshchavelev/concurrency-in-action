#include <atomic>
#include <cassert>
#include <thread>

std::atomic<bool> x{false};
std::atomic<bool> y{false};
std::atomic<int>  z{0};

void write_x_then_y() {
    x.store(true, std::memory_order_relaxed);
    y.store(true, std::memory_order_relaxed);
}

void read_y_then_x() {
    while (!y.load(std::memory_order_relaxed)) {
    }
    if (x.load(std::memory_order_relaxed)) {
        ++z;
    }
}

int main() {
    std::jthread t1(write_x_then_y);
    std::jthread t2(read_y_then_x);

    t1.join();
    t2.join();

    // The assert might succeed because, due to `memory_order_relaxed`, the order of operations is not guaranteed, which could
    // result in `x` still being `false` even after `y` has become `true`.
    assert(z.load() != 0);

    return 0;
}