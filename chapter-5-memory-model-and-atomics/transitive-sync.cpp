// This example demonstrates how data visibility is synchronized across threads

#include <atomic>
#include <cassert>
#include <thread>

std::atomic<int> data[3];
std::atomic<int> flag{0};

void thread1() {
    data[0].store(0, std::memory_order_relaxed);
    data[1].store(1, std::memory_order_relaxed);
    data[2].store(2, std::memory_order_relaxed);
    // `memory_order_release` guarantees that all *write* operations performed *before* it will be visible to the second thread in
    // the same order in which they were executed
    flag.store(1, std::memory_order_release);
}

void thread2() {
    int expected = 1;

    // If thread 1 has indicated that the data is ready (the flag value is “expected”), write the value 2 to the flag.
    // If the comparison succeeds, the `memory_order_acq_rel` memory model is used because Thread 2 must see the flag’s value
    // after it is written by Thread 1, and then write the new value in such a way that any write operations to `data` by Thread 3
    // are guaranteed to be visible before the new value is written to the flag. If the comparison fails, the data remains
    // unchanged, so `memory_order_relaxed` can be used.
    while (!flag.compare_exchange_strong(expected, 2, std::memory_order_acq_rel, std::memory_order_relaxed)) {
        expected = 1;
    }
}

void thread3() {
    while (flag.load(std::memory_order_acquire) != 2) {
    }

    assert(data[0].load(std::memory_order_relaxed) == 0);
    assert(data[1].load(std::memory_order_relaxed) == 1);
    assert(data[2].load(std::memory_order_relaxed) == 2);
}

int main() {
    std::jthread t1(thread1);
    std::jthread t2(thread2);
    std::jthread t3(thread3);
}
