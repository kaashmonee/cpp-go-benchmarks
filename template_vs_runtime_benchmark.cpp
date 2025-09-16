#include <iostream>
#include <chrono>
#include <vector>

// Template version - boolean parameter
template <bool DoCheck>
void process_array_template(std::vector<int> &data)
{
    for (size_t i = 0; i < data.size(); ++i)
    {
        if constexpr (DoCheck)
        {
            // Bounds checking version
            if (i < data.size())
            { // This check is actually optimized away completely
                data[i] *= 2;
            }
        }
        else
        {
            // No bounds checking
            data[i] *= 2;
        }
    }
}

// Runtime version - boolean parameter
void process_array_runtime(std::vector<int> &data, bool doCheck)
{
    for (size_t i = 0; i < data.size(); ++i)
    {
        if (doCheck)
        {
            // Bounds checking version
            if (i < data.size())
            { // This check is still performed at runtime
                data[i] *= 2;
            }
        }
        else
        {
            // No bounds checking
            data[i] *= 2;
        }
    }
}

// Template version - integer parameter for loop unrolling
template <int UnrollFactor>
void process_array_unrolled(std::vector<int> &data)
{
    const size_t size = data.size();
    size_t i = 0;

    // Process in chunks of UnrollFactor
    for (; i + UnrollFactor <= size; i += UnrollFactor)
    {
        if constexpr (UnrollFactor >= 1)
            data[i] *= 2;
        if constexpr (UnrollFactor >= 2)
            data[i + 1] *= 2;
        if constexpr (UnrollFactor >= 3)
            data[i + 2] *= 2;
        if constexpr (UnrollFactor >= 4)
            data[i + 3] *= 2;
        // For larger unroll factors, add more lines
    }

    // Handle remaining elements
    for (; i < size; ++i)
    {
        data[i] *= 2;
    }
}

// Benchmark function
template <typename Func, typename... Args>
double benchmark(Func func, std::vector<int> &data, Args &&...args)
{
    auto start = std::chrono::high_resolution_clock::now();

    // Run the function multiple times for more accurate timing
    for (int i = 0; i < 1000; ++i)
    {
        func(data, std::forward<Args>(args)...);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}

int main()
{
    const int SIZE = 100000;
    std::vector<int> data(SIZE, 1);
    std::vector<int> data_copy = data;

    // Benchmark template vs runtime boolean parameter
    double template_true_time = benchmark(process_array_template<true>, data);
    data = data_copy;

    double template_false_time = benchmark(process_array_template<false>, data);
    data = data_copy;

    double runtime_true_time = benchmark(process_array_runtime, data, true);
    data = data_copy;

    double runtime_false_time = benchmark(process_array_runtime, data, false);
    data = data_copy;

    // Benchmark different unroll factors
    double unroll1_time = benchmark(process_array_unrolled<1>, data);
    data = data_copy;

    double unroll2_time = benchmark(process_array_unrolled<2>, data);
    data = data_copy;

    double unroll4_time = benchmark(process_array_unrolled<4>, data);

    // Calculate percentage improvements
    double template_true_vs_runtime_true = ((runtime_true_time - template_true_time) / runtime_true_time) * 100.0;
    double template_false_vs_runtime_false = ((runtime_false_time - template_false_time) / runtime_false_time) * 100.0;

    double unroll2_vs_unroll1 = ((unroll1_time - unroll2_time) / unroll1_time) * 100.0;
    double unroll4_vs_unroll1 = ((unroll1_time - unroll4_time) / unroll1_time) * 100.0;

    // Print results
    std::cout << "Boolean Parameter Benchmarks (1000 iterations on array of " << SIZE << " elements):\n";
    std::cout << "Template with DoCheck=true: " << template_true_time << " ms\n";
    std::cout << "Runtime with doCheck=true: " << runtime_true_time << " ms\n";
    std::cout << "  Improvement: " << template_true_vs_runtime_true << "% faster with template\n\n";

    std::cout << "Template with DoCheck=false: " << template_false_time << " ms\n";
    std::cout << "Runtime with doCheck=false: " << runtime_false_time << " ms\n";
    std::cout << "  Improvement: " << template_false_vs_runtime_false << "% faster with template\n";
    std::cout << "Unroll factor 1: " << unroll1_time << " ms (baseline)\n";
    std::cout << "Unroll factor 2: " << unroll2_time << " ms (" << unroll2_vs_unroll1 << "% improvement)\n";
    std::cout << "Unroll factor 4: " << unroll4_time << " ms (" << unroll4_vs_unroll1 << "% improvement)\n";

    return 0;
}