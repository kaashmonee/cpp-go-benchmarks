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

