#include <atomic>
#include <iostream>

int main() {
    std::atomic<int> value;

    std::cout << std::boolalpha;
    std::cout << std::atomic<int>::is_always_lock_free << std::endl; // true expected (constexpr static)
    std::cout << value.is_lock_free() << std::endl;                  // true expected (runtime)

    return 0;
}