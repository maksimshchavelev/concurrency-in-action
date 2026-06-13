#include <iostream>
#include <thread>

struct Logger {
    void log() {
        std::cout << "log(): hello world\n";
    }
};

int main() {
    Logger logger;

    std::jthread t1(&Logger::log, logger);           // Copy. The lifetime doesn't matter
    std::jthread t2(&Logger::log, &logger);          // Pointer. The logger's lifetime must be longer than the thread's lifetime
    std::jthread t3(&Logger::log, std::ref(logger)); // Reference. The logger's lifetime must be longer than the thread's lifetime
}
