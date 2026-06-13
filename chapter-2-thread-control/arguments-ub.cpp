#include <iostream>
#include <thread>

void f(const std::string& s) {
    std::cout << s;
}

void oops() {
    char buffer[1024];
    std::sprintf(buffer, "%i", 1024);

    // A classic "use after return" stack bug. A pointer to the buffer is copied into the internal storage of `std::thread`, but
    // by the time a `std::string` is constructed from the raw string, the pointer may be pointing to data that has already been
    // removed from the stack after the function returned.
    std::thread t(f, buffer);
    t.detach();
}

int main() {
    oops();

    std::this_thread::sleep_for(std::chrono::milliseconds{20});

    return 0;
}
