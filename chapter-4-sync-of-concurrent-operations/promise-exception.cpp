#include <future>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

int main() {
    std::promise<int> promise;
    std::future<int>  future = promise.get_future();

    std::jthread t([promise = std::move(promise)]() mutable {
        std::this_thread::sleep_for(1s);

        try {
            throw std::runtime_error("BOOM! Something went wrong in the worker thread.");
        } catch (...) {
            // std::current_exception() returns a smart pointer (exception_ptr) to the current “flying” exception object. The
            // runtime determines where the exception object is located. By this point, the stack has already been unwound.
            promise.set_exception(std::current_exception());
        }
    });

    try {
        [[maybe_unused]] int result = future.get(); // That's where the exception will be thrown
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}