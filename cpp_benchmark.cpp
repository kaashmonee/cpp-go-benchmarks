#include <iostream>
#include <vector>
#include <string>
#include <chrono>

// A large object that's expensive to copy
class LargeObject {
private:
    std::vector<int> data;
public:
    LargeObject(size_t size = 1000000) : data(size, 42) {}
    
    // Copy constructor - expensive!
    LargeObject(const LargeObject& other) : data(other.data) {
        // Intentionally do nothing else, copying is already expensive
    }
    
    // Move constructor - cheap!
    LargeObject(LargeObject&& other) noexcept : data(std::move(other.data)) {}
    
    // Copy assignment
    LargeObject& operator=(const LargeObject& other) {
        data = other.data;
        return *this;
    }
    
    // Move assignment
    LargeObject& operator=(LargeObject&& other) noexcept {
        data = std::move(other.data);
        return *this;
    }
};

// Function that returns by value - will copy if move semantics aren't used
LargeObject createLargeObject() {
    LargeObject obj;
    return obj;
}

// Test copy performance
void testCopy(int iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        LargeObject source;
        LargeObject dest = source; // Explicit copy
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Copy time: " << elapsed.count() << " seconds" << std::endl;
}

// Test move performance
void testMove(int iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        LargeObject source;
        LargeObject dest = std::move(source); // Explicit move
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Move time: " << elapsed.count() << " seconds" << std::endl;
}

// Test return value optimization with move semantics
void testReturnValue(int iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; i++) {
        LargeObject obj = createLargeObject(); // Will use move semantics
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Return value (with move) time: " << elapsed.count() << " seconds" << std::endl;
}

int main() {
    int iterations = 100;
    
    std::cout << "Running benchmarks with " << iterations << " iterations each..." << std::endl;
    std::cout << "Each operation handles a vector of 1 million integers" << std::endl;
    
    testCopy(iterations);
    testMove(iterations);
    testReturnValue(iterations);
    
    return 0;
}