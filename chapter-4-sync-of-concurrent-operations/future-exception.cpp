#include <future>
#include <iostream>

using namespace std::chrono_literals;

int main() {
    auto future = std::async(std::launch::async, []() {
        // An exception will be thrown when `future.get()` is called.
        throw std::runtime_error("BOOM!");
        return 0;
    });

    std::this_thread::sleep_for(500ms);

    try {
        // When `get()` is called, any exception (if there was one) is propagated outward
        [[maybe_unused]] auto res = future.get();
    } catch (std::runtime_error& e) {
        std::cout << "exception: " << e.what();
    }
}