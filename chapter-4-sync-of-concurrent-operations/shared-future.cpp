#include <future>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

int main() {
    // If you use `std::future` from multiple threads, a race condition will occur due to shared state. To avoid this, you should
    // use `std::shared_future`, which allows multiple threads to operate on it—for example, calling the `get()` method from
    // multiple threads—but **each thread must have its own copy of the `shared_future`!**

    std::promise<int>       promise;
    std::shared_future<int> future = promise.get_future();

    std::mutex mutex; // for using with std::cout

    auto f = [future, &mutex]() {
        std::lock_guard lock(mutex);
        std::cout << "Got value: " << future.get() << std::endl;
    };

    std::jthread t1(f);
    std::jthread t2(f);

    std::this_thread::sleep_for(1s);

    promise.set_value(100);

    return 0;
}