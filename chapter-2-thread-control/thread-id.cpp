#include <iostream>
#include <thread>

int main() {
    std::thread t; // No thread is created in the kernel

    std::cout << std::boolalpha;

    std::cout << "t.joinable() == " << t.joinable() << '\n'; // false. The object is not associated with the execution thread
    std::cout << t.get_id() << "\n\n";                       // id of a non-executing thread

    std::thread g([]() { std::this_thread::sleep_for(std::chrono::milliseconds{100}); });

    std::cout << "g.joinable() == " << g.joinable() << '\n'; // true
    std::cout << g.get_id() << '\n'; // id. The thread is associated with the kernel's own thread (the execution thread)
    g.join();

    std::cout << g.get_id() << '\n'; // id of a non-executing thread. Thread finished

    return 0;
}
