#include <iostream>
#include <thread>

void oops() {
    std::thread t([]() {
        int counter{0};

        for (int i = 0; i < 10'000'000; ++i) {
            ++counter;
        }
    });

    throw 0;

    t.join();
}

int main() {
    try {
        oops();
    } catch (...) {
        // This will not be printed because the thread destructor will call `std::terminate()` due to the thread being joined or
        // detached
        // Fix: use std::jthread or join thread manually before exception
        std::cout << "Caught!" << std::endl;
    }

    return 0;
}