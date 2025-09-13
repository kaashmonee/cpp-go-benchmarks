all: cpp_benchmark go_benchmark

cpp_benchmark: cpp_benchmark.cpp
	g++ -std=c++14 -O2 cpp_benchmark.cpp -o cpp_benchmark

go_benchmark: go_benchmark.go
	go build -o go_benchmark go_benchmark.go

run_benchmarks: cpp_benchmark go_benchmark
	@echo "Running C++ benchmark..."
	./cpp_benchmark
	@echo ""
	@echo "Running Go benchmark..."
	./go_benchmark

clean:
	rm -f cpp_benchmark go_benchmark