#include <thread>

using namespace std::chrono_literals;

int main() {
    std::thread t1;
    std::thread t2;

    t1 = std::thread([&]() {
        std::this_thread::sleep_for(50ms);
        t2.join();
    });

    t2 = std::thread([&]() {
        std::this_thread::sleep_for(50ms);
        t1.join();
    });

    std::this_thread::sleep_for(200ms);

    // Both threads will still be joinable because a deadlock will occur, and neither thread will be joined. Thread 1 is waiting
    // for Thread 2 to finish, but Thread 2 is waiting for Thread 1 to finish. It's a vicious cycle. So the program will crash
    // because neither thread has been joined by the time the program terminates.

    return 0;
}