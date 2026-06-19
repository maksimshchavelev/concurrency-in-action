#include <iostream>
#include <thread>

int main() {
    size_t counter{0};

    std::thread t1([&counter]() {
        for (size_t i = 0; i < 20'000'000; ++i) {
            ++counter;
        }
    });

    std::thread t2([&counter]() {
        for (size_t i = 0; i < 20'000'000; ++i) {
            ++counter;
        }
    });

    t1.join();
    t2.join();

    std::cout << counter << std::endl; // Different values each time

    return 0;
}
