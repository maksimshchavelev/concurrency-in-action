#include <atomic>
#include <cassert>
#include <thread>

std::atomic<int>  data[3];
std::atomic<bool> flag{false};

void thread1() {
    data[0].store(0, std::memory_order_relaxed);
    data[1].store(1, std::memory_order_relaxed);
    data[2].store(2, std::memory_order_relaxed);
    // `memory_order_release` guarantees that all *write* operations performed *before* it will be visible to the second thread in
    // the same order in which they were executed
    flag.store(true, std::memory_order_release);
}

void thread2() {
    // If the flag is set to true, it is guaranteed that the data will definitely be valid.
    while (!flag.load(std::memory_order_acquire)) {
    }

    assert(data[0].load(std::memory_order_relaxed) == 0);
    assert(data[1].load(std::memory_order_relaxed) == 1);
    assert(data[2].load(std::memory_order_relaxed) == 2);
}

int main() {
    std::jthread t1(thread1);
    std::jthread t2(thread2);
}