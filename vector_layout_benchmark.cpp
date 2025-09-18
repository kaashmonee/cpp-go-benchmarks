#include <vector>
#include <chrono>
#include <iostream>
#include <iomanip>

// Using large dimensions to make the performance difference more noticeable
const size_t WIDTH = 1000;
const size_t DEPTH = 1000;
const size_t NUM_ITERATIONS = 10000000;

// Helper function to prevent compiler from optimizing away the operations
void prevent_optimize(uint32_t val)
{
    if (val == 0xDEADBEEF)
    {
        std::cout << val; // Will never happen, but compiler doesn't know that
    }
}

double benchmark_2d_vector()
{
    // Vector of vectors approach
    std::vector<std::vector<uint32_t>> matrix2d(WIDTH, std::vector<uint32_t>(DEPTH, 0));

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t iter = 0; iter < NUM_ITERATIONS; iter++)
    {
        size_t i = iter % WIDTH;
        size_t j = iter % DEPTH;
        matrix2d[i][j]++;
        prevent_optimize(matrix2d[i][j]);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    return duration / 1000.0; // Convert to milliseconds
}

double benchmark_flat_vector()
{
    // Flattened vector approach
    std::vector<uint32_t> matrix1d(WIDTH * DEPTH, 0);

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t iter = 0; iter < NUM_ITERATIONS; iter++)
    {
        size_t i = iter % WIDTH;
        size_t j = iter % DEPTH;
        matrix1d[i * DEPTH + j]++;
        prevent_optimize(matrix1d[i * DEPTH + j]);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    return duration / 1000.0; // Convert to milliseconds
}

int main()
{
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Running benchmarks with dimensions " << WIDTH << "x" << DEPTH
              << " and " << NUM_ITERATIONS << " iterations\n\n";

    // Run each benchmark multiple times to get more stable results
    const int NUM_RUNS = 5;
    double total_2d = 0, total_flat = 0;

    for (int i = 0; i < NUM_RUNS; i++)
    {
        double time_2d = benchmark_2d_vector();
        double time_flat = benchmark_flat_vector();

        std::cout << "Run " << (i + 1) << ":\n";
        std::cout << "  2D Vector time:     " << time_2d << " ms\n";
        std::cout << "  Flattened time:     " << time_flat << " ms\n";
        double percent_faster = ((time_2d - time_flat) / time_2d) * 100;
        std::cout << "  Improvement:        " << percent_faster << "% faster\n\n";

        total_2d += time_2d;
        total_flat += time_flat;
    }

    // Print averages
    double avg_2d = total_2d / NUM_RUNS;
    double avg_flat = total_flat / NUM_RUNS;

    std::cout << "Average results:\n";
    std::cout << "  2D Vector time:     " << avg_2d << " ms\n";
    std::cout << "  Flattened time:     " << avg_flat << " ms\n";
    double avg_percent_faster = ((avg_2d - avg_flat) / avg_2d) * 100;
    std::cout << "  Average improvement: " << avg_percent_faster << "% faster\n";

    return 0;
}

// flattened vs. 2d vector benchmarks:
// Going to go with the 2d vector for now...
// g++ -std=c++14 -O3 vector_layout_benchmark.cpp -o vector_benchmark
// Running benchmarks with dimensions 1000x1000 and 10000000 iterations

// Run 1:
//   2D Vector time:     18.58 ms
//   Flattened time:     15.79 ms
//   Improvement:        14.98% faster

// Run 2:
//   2D Vector time:     19.05 ms
//   Flattened time:     15.87 ms
//   Improvement:        16.69% faster

// Run 3:
//   2D Vector time:     18.71 ms
//   Flattened time:     15.29 ms
//   Improvement:        18.26% faster

// Run 4:
//   2D Vector time:     18.75 ms
//   Flattened time:     15.87 ms
//   Improvement:        15.34% faster

// Run 5:
//   2D Vector time:     19.13 ms
//   Flattened time:     14.99 ms
//   Improvement:        21.64% faster

// Average results:
//   2D Vector time:     18.84 ms
//   Flattened time:     15.56 ms
//   Average improvement: 17.40% faster