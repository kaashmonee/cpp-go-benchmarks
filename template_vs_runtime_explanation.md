# Compile-Time vs. Runtime Efficiency with Template Parameters

When using non-class template parameters (like booleans or integers) instead of runtime parameters, there are several significant efficiency advantages:

## 1. Dead Code Elimination

When you use a boolean template parameter like `template <bool DoCheck>`, the compiler knows the exact value of `DoCheck` at compile time. With modern C++ features like `if constexpr`, the compiler completely eliminates code branches that aren't taken:

```cpp
if constexpr (DoCheck) {
    // This code is completely removed from the compiled binary
    // when DoCheck is false
}
```

This is different from runtime conditionals, which still need to evaluate the condition and branch at runtime:

```cpp
if (doCheck) {
    // This branch check always happens at runtime
}
```

## 2. Compile-Time Constant Folding and Propagation

The compiler can propagate known template parameter values throughout the function and perform calculations at compile time. This can lead to:

- Simplified instructions
- Removed unnecessary operations
- Pre-computed values

## 3. Specialized Code Generation

For each different template parameter value, the compiler generates a completely separate function specialized for that specific value. This allows for:

- Better inlining decisions
- CPU-specific optimizations
- Better register allocation

## 4. Loop Unrolling Opportunities

Integer template parameters enable precise loop unrolling with compile-time known factors:

```cpp
template <int Factor>
void process() {
    // The compiler can unroll this loop exactly to the Factor value
    for (int i = 0; i < Factor; i++) {
        // Processing
    }
}
```

## 5. Real-World Performance Impact

The benchmark code demonstrates these differences. You'll typically see:

1. The template version with `DoCheck=false` is often significantly faster than the runtime version with `doCheck=false` because there's no branch evaluation.

2. The template version with `DoCheck=true` is usually faster than the runtime version with `doCheck=true` because the compiler can better optimize the known code path.

3. Loop unrolling with larger unroll factors often shows improved performance up to a certain point.

## 6. Disadvantages

The primary disadvantage is binary size. Each template instantiation creates a separate function in the binary, which can lead to code bloat in large applications with many template parameter combinations.

## When to Use Template Parameters vs. Runtime Parameters

- Use template parameters when:
  - The value is known at compile time
  - The code branches significantly based on the parameter
  - Performance is critical for the function
  - The number of different parameter values is limited

- Use runtime parameters when:
  - Values are only known at runtime
  - Code bloat is a concern
  - The same code path works efficiently for all values
  - Flexibility to change behavior without recompilation is needed