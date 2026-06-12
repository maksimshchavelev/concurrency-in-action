#include <iostream>
#include <thread>

struct background_task {
    void operator ()() const {
        std::cout << "Background task works!\n";
    }
};

int main() {
    std::thread t(background_task{});
    t.join();
}
