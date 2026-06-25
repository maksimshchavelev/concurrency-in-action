#include <chrono>
#include <iostream>

int main() {
    auto system_time = std::chrono::system_clock::now();
    auto steady_time = std::chrono::steady_clock::now();

    std::cout << std::boolalpha;

    std::cout << std::chrono::system_clock::time_point::min() << std::endl; // The earliest time point
    std::cout << std::chrono::system_clock::time_point::max() << std::endl; // The latest time point

    std::cout << system_time.time_since_epoch() << std::endl; // The time elapsed since 1970-01-01 00:00:00 UTC
    std::cout << steady_time.time_since_epoch() << std::endl; // The time that has elapsed since the operating system was launched

    std::cout << std::chrono::system_clock::is_steady << std::endl; // Is the clock accurate?
    std::cout << std::chrono::steady_clock::is_steady << std::endl; // Is the clock accurate?

    return 0;
}