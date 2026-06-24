#include <future>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

int main() {
    std::promise<int> promise;
    std::future<int>  future = promise.get_future();

    std::jthread t([promise = std::move(promise)]() mutable {
        std::this_thread::sleep_for(1s);
        promise.set_value(5);
    });

    std::cout << future.get() << std::endl; // 5

    return 0;
}