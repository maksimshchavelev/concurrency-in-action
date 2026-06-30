#include <atomic>
#include <iostream>

// The structure will be 16 bytes in size. On most platforms, such structures will not be lock-free, so the compiler will use the
// atomic library, which must be explicitly linked with the executable in CMake.
// To use your type with `atomic`, it must have a **trivial copy constructor**, a **trivial copy assignment operator**, and a
// **trivial destructor**
struct MyType {
    int    a{0};
    double b{0.0};
};

int main() {
    std::atomic<MyType> value;
    value.store({0, 5.4});
    MyType data = value.load();

    std::cout << data.a << ' ' << data.b;

    return 0;
}