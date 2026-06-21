#include <iostream>
#include <memory>
#include <mutex>

struct BigResource {
    BigResource() {
        std::cout << "Init big resource..." << std::endl;
    }

    void print() {
        std::cout << "print()" << std::endl;
    }
};

std::shared_ptr<BigResource> resource;
std::once_flag               resource_flag;

void init_resource() {
    resource = std::make_shared<BigResource>();
}

void foo() {
    std::call_once(resource_flag, init_resource);
    resource->print();
}

int main() {
    foo();
    foo();
}
