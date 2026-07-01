#include <atomic>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

int main() {
    // The "happens-before" synchronization pattern:
    // 1. Write the data first.
    // 2. Then set the ready flag with memory_order_release.
    // 3. The other thread reads the flag with memory_order_acquire.
    // If the acquire-load sees the flag as true, the data is guaranteed to be visible.
    // Using an incorrect memory order (e.g., relaxed) allows reordering: the flag may
    // become visible before the data store, leading to a data race and undefined behavior.

    int               data{0};
    std::atomic<bool> flag;

    std::jthread producer([&]() {
        std::this_thread::sleep_for(200ms);
        data = 10;
        flag.store(true, std::memory_order_release);
    });

    std::jthread consumer([&]() {
        while (!flag.load(std::memory_order_acquire)) {
        }

        int value = data; // value is 10
        std::cout << value << std::endl;
    });

    producer.join();
    consumer.join();

    return 0;
}