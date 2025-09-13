package main

import (
	"fmt"
	"time"
)

// Roughly equivalent to the C++ LargeObject
type LargeObject struct {
	data []int
}

// Create a new large object
func newLargeObject(size int) LargeObject {
	data := make([]int, size)
	for i := range data {
		data[i] = 42
	}
	return LargeObject{data: data}
}

// Function that returns by value - in Go this is already efficient
func createLargeObject() LargeObject {
	obj := newLargeObject(1000000)
	return obj
}

// Test copy performance
func testCopy(iterations int) {
	start := time.Now()

	for i := 0; i < iterations; i++ {
		source := newLargeObject(1000000)
		
		// In Go, this creates a new struct, but the underlying slice 
		// shares the same backing array by default - NOT a full copy!
		dest := source
		
		// To force a full copy like in C++, uncomment these lines:
		// destData := make([]int, len(source.data))
		// copy(destData, source.data)
		// dest.data = destData
		
		// Prevent optimizations
		_ = dest
	}

	elapsed := time.Since(start)
	fmt.Printf("Copy time: %v seconds\n", elapsed.Seconds())
}

// Test "move" performance (Go doesn't have explicit moves)
func testMove(iterations int) {
	start := time.Now()

	for i := 0; i < iterations; i++ {
		source := newLargeObject(1000000)
		
		// This is the same as above - in Go this is already efficient
		dest := source
		
		// In Go we have no way to invalidate source - this is handled by GC
		
		// Prevent optimizations
		_ = dest
	}

	elapsed := time.Since(start)
	fmt.Printf("'Move' time: %v seconds\n", elapsed.Seconds())
}

// Test return value (already efficient in Go)
func testReturnValue(iterations int) {
	start := time.Now()

	for i := 0; i < iterations; i++ {
		obj := createLargeObject()
		_ = obj
	}

	elapsed := time.Since(start)
	fmt.Printf("Return value time: %v seconds\n", elapsed.Seconds())
}

// To simulate a true deep copy (like C++ without move semantics)
func testForcedDeepCopy(iterations int) {
	start := time.Now()

	for i := 0; i < iterations; i++ {
		source := newLargeObject(1000000)
		
		// Create a true deep copy
		destData := make([]int, len(source.data))
		copy(destData, source.data)
		dest := LargeObject{data: destData}
		
		// Prevent optimizations
		_ = dest
	}

	elapsed := time.Since(start)
	fmt.Printf("Forced deep copy time: %v seconds\n", elapsed.Seconds())
}

func main() {
	iterations := 100
	
	fmt.Printf("Running benchmarks with %d iterations each...\n", iterations)
	fmt.Println("Each operation handles a slice of 1 million integers")
	
	testCopy(iterations)
	testMove(iterations)
	testReturnValue(iterations)
	testForcedDeepCopy(iterations) // This is like C++ copy (without move)
}