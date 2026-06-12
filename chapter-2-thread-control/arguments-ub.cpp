#include <thread>
#include <iostream>

void f(const std::string& s) {
    std::cout << s;
}

void oops() {
    char buffer[1024];
    std::sprintf(buffer, "%i", 1024);

    std::thread t(f, buffer);
    t.detach();
}

int main() {
    oops();

    std::this_thread::sleep_for(std::chrono::milliseconds{20});

    return 0;
}

