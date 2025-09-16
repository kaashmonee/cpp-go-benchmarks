# cpp-go-benchmarks


## running instructions


`make run_benchmarks`


## results

```
g++ -std=c++14 -O2 cpp_benchmark.cpp -o cpp_benchmark
go build -o go_benchmark go_benchmark.go
Running C++ benchmark...
./cpp_benchmark
Running benchmarks with 100 iterations each...
Each operation handles a vector of 1 million integers
Copy time: 0.460536 seconds
Move time: 0.0359809 seconds
Return value (with move) time: 0.0345974 seconds

Running Go benchmark...
./go_benchmark
Running Go benchmarks with 100 iterations each...
Each operation handles a slice of 1 million integers
Go version: go1.25.1
Pointer semantics (shallow copy) time: 0.125397025 seconds
Value semantics (deep copy) time: 0.25320995 seconds
Simulated 'move' time: 0.131937821 seconds
Return value time: 0.11745473 seconds

Slice-specific tests (Go's built-in optimization):
Slice copy time (shared backing array): 0.136339759 seconds
Slice deep copy time (new backing array): 0.253797554 seconds
```

# conclusion

FINE. i guess i buy this. i guess i buy why i should care about movesemantics. 4x better is still pretty insane. FINE. ugh

## templated functions benchmarks results

```
Boolean Parameter Benchmarks (1000 iterations on array of 100000 elements):

Template with DoCheck=true: 33.7282 ms

Runtime with doCheck=true: 14.592 ms

  Improvement: -131.141% faster with template

Template with DoCheck=false: 17.6207 ms

Runtime with doCheck=false: 14.6132 ms

  Improvement: -20.581% faster with template

Unroll factor 1: 11.8006 ms (baseline)

Unroll factor 2: 11.9554 ms (-1.31182% improvement)

Unroll factor 4: 11.8346 ms (-0.288486% improvement)
```

seems like the templated functions are not ALWAYS faster even though they're evaluated
at compile time. this contrived example is not just contrived, it's actually an anti-pattern
can't help but feel like this is an example of contrived difficulty and/or red herring

