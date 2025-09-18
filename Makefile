all: cpp_benchmark go_benchmark template_benchmark vector_benchmark

cpp_benchmark: cpp_benchmark.cpp
	g++ -std=c++14 -O2 cpp_benchmark.cpp -o cpp_benchmark

go_benchmark: go_benchmark.go
	go build -o go_benchmark go_benchmark.go

template_benchmark: template_vs_runtime_benchmark.cpp
	g++ -std=c++11 -O3 template_vs_runtime_benchmark.cpp -o template_benchmark

run_benchmarks: cpp_benchmark go_benchmark
	@echo "Running C++ benchmark..."
	./cpp_benchmark
	@echo ""
	@echo "Running Go benchmark..."
	./go_benchmark

run_template_benchmark: template_benchmark
	@echo "Running Template vs Runtime benchmark..."
	./template_benchmark

vector_benchmark: vector_layout_benchmark.cpp
	g++ -std=c++14 -O3 vector_layout_benchmark.cpp -o vector_benchmark

run_vector_benchmark: vector_benchmark
	@echo "Running Vector Layout benchmark..."
	./vector_benchmark

clean:
	rm -f cpp_benchmark go_benchmark template_benchmark vector_benchmark
