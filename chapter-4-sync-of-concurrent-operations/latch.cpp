#include <format>
#include <iostream>
#include <latch>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

/// Thread-safe print
void print(const std::string& message) {
    static std::mutex mutex;
    std::lock_guard   lock(mutex);
    std::cout << message << std::endl;
}

int main() {
    // The latch allows you to wait until the counter reaches zero (for example, until all threads are ready). The initial value
    // of the counter is specified in the constructor
    std::latch latch(1);

    std::vector<std::jthread> threads;

    for (unsigned i = 0; i < 8; ++i) {
        threads.emplace_back([&latch]() {
            // `std::latch::wait` waits until the latch counter reaches zero
            latch.wait();
            print(std::format("Thread {} started", std::this_thread::get_id()));
        });
    }

    print("Waiting...");

    std::this_thread::sleep_for(1500ms);

    // Decrease the counter value by 1
    latch.count_down();

    return 0;
}