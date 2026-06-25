#include <atomic>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

int main() {
    std::atomic_flag flag = ATOMIC_FLAG_INIT; // Set the flag to false

    std::cout << std::boolalpha;

    std::cout << flag.test() << std::endl;         // false
    std::cout << flag.test_and_set() << std::endl; // returns false; sets the flag to true
    std::cout << flag.test() << std::endl;         // true

    flag.clear(); // set the flag to false

    // You can use a flag as an alternative to a condition variable
    // It is lock-free and has no false wakes
    std::jthread t([&flag]() {
        std::this_thread::sleep_for(1s);
        flag.test_and_set();
        flag.notify_one();
    });

    flag.wait(false);

    std::cout << "the flag is set!" << std::endl;

    return 0;
}