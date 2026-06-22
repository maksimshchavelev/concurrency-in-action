#include <future>
#include <iostream>

using namespace std::chrono_literals;

void async() {
    // `std::launch::async` will run the task in a new thread
    auto future = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(250ms);
        return 10;
    });

    auto res = future.get();
    std::cout << "Got result (async): " << res << std::endl;
}

void deferred() {
    // std::launch::deferred will not run the task on a new thread, but will use the current thread. The task will be run only
    // when the `get()` method of the future is called.
    auto future = std::async(std::launch::deferred, []() {
        std::this_thread::sleep_for(250ms);
        return 20;
    });

    auto res = future.get();
    std::cout << "Got result (deffered): " << res << std::endl;
}

int main() {
    async();
    deferred();
}