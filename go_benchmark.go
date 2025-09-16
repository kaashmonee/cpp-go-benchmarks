package main

import (
	"fmt"
	"runtime"
	"time"
)

// LargeData simulates a large chunk of memory like the C++ vector
type LargeData struct {
	data []int
}

// LargeObject demonstrates the difference between pointer semantics and value semantics
type LargeObject struct {
	// We'll use a pointer to data to better demonstrate the issue
	data *LargeData
}

// Create a new large object
func newLargeObject(size int) LargeObject {
	data := make([]int, size)
	for i := range data {
		data[i] = 42
	}
	return LargeObject{
		data: &LargeData{data: data},
	}
}

// Create a large object that will be returned
func createLargeObject() LargeObject {
	obj := newLargeObject(1000000)
	return obj
}

// Test value semantics copy performance (similar to C++ copy without move)
func testValueSemanticsCopy(iterations int) {
	start := time.Now()

	for i := 0; i < iterations; i++ {
		source := newLargeObject(1000000)
		
		// Deep copy - equivalent to C++ copy constructor
		destData := make([]int, len(source.data.data))
		copy(destData, source.data.data)
		dest := LargeObject{
			data: &LargeData{data: destData},
		}
		
		// Prevent optimizations
		_ = dest
	}

	elapsed := time.Since(start)
	fmt.Printf("Value semantics (deep copy) time: %v seconds\n", elapsed.Seconds())
}

// Test pointer semantics (similar to C++ move or reference semantics)
func testPointerSemantics(iterations int) {
	start := time.Now()

	for i := 0; i < iterations; i++ {
		source := newLargeObject(1000000)
		
		// Pointer copy - just copying the reference, similar to C++ move
		dest := source
		
		// Prevent optimizations
		_ = dest
	}

	elapsed := time.Since(start)
	fmt.Printf("Pointer semantics (shallow copy) time: %v seconds\n", elapsed.Seconds())
}

// Simulate taking ownership in Go (closest to C++ move semantics)
func testSimulatedMove(iterations int) {
	start := time.Now()

	for i := 0; i < iterations; i++ {
		source := newLargeObject(1000000)
		
		// "Move" semantics in Go - copy the pointer and null the source
		dest := source
		source.data = nil // Explicitly abandon the source data
		
		// Prevent optimizations
		_ = dest
	}

	elapsed := time.Since(start)
	fmt.Printf("Simulated 'move' time: %v seconds\n", elapsed.Seconds())
}

// Test return value efficiency
func testReturnValue(iterations int) {
	start := time.Now()

	for i := 0; i < iterations; i++ {
		obj := createLargeObject()
		_ = obj
	}

	elapsed := time.Since(start)
	fmt.Printf("Return value time: %v seconds\n", elapsed.Seconds())
}

// To make the comparison more accurate, also test a case with
// real value semantics like in C++
type ValueLargeObject struct {
	// Embedded data - value semantics like C++
	data []int
}

func newValueLargeObject(size int) ValueLargeObject {
	data := make([]int, size)
	for i := range data {
		data[i] = 42
	}
	return ValueLargeObject{data: data}
}

func testSliceCopy(iterations int) {
	start := time.Now()

	for i := 0; i < iterations; i++ {
		source := newValueLargeObject(1000000)
		
		// This copies the slice header, but not the backing array
		dest := source
		
		// Prevent optimizations
		_ = dest
	}

	elapsed := time.Since(start)
	fmt.Printf("Slice copy time (shared backing array): %v seconds\n", elapsed.Seconds())
}

func testForceSliceDeepCopy(iterations int) {
	start := time.Now()

	for i := 0; i < iterations; i++ {
		source := newValueLargeObject(1000000)
		
		// Force a complete copy of the slice's backing array
		destData := make([]int, len(source.data))
		copy(destData, source.data)
		dest := ValueLargeObject{data: destData}
		
		// Prevent optimizations
		_ = dest
	}

	elapsed := time.Since(start)
	fmt.Printf("Slice deep copy time (new backing array): %v seconds\n", elapsed.Seconds())
}

func main() {
	iterations := 100
	
	fmt.Printf("Running Go benchmarks with %d iterations each...\n", iterations)
	fmt.Println("Each operation handles a slice of 1 million integers")
	fmt.Printf("Go version: %s\n", runtime.Version())
	
	// Run GC before tests to reduce interference
	runtime.GC()
	
	testPointerSemantics(iterations)     // Similar to C++ reference or move
	testValueSemanticsCopy(iterations)   // Similar to C++ copy without move
	testSimulatedMove(iterations)        // Closest to C++ move semantics
	testReturnValue(iterations)          // Return value efficiency
	
	fmt.Println("\nSlice-specific tests (Go's built-in optimization):")
	testSliceCopy(iterations)            // Go's default slice behavior
	testForceSliceDeepCopy(iterations)   // Forcing deep copies like C++
}