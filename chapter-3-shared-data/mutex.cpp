#include <iostream>
#include <mutex>
#include <thread>

int main() {
    size_t     counter{0};
    std::mutex mutex;

    std::thread t1([&counter, &mutex]() {
        for (size_t i = 0; i < 10'000'000; ++i) {
            std::lock_guard guard(mutex); // Using RAII instead of manually locking and unlocking
            ++counter;
        }
    });

    std::thread t2([&counter, &mutex]() {
        for (size_t i = 0; i < 10'000'000; ++i) {
            std::lock_guard guard(mutex); // Using RAII instead of manually locking and unlocking
            ++counter;
        }
    });

    t1.join();
    t2.join();

    std::cout << std::boolalpha << (counter == 20'000'000); // true

    return 0;
}
