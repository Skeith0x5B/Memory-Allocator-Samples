/*
===============================================================================

	Sample Code for MSc Application
	===============================
	File		:	MSC_Main.cpp
	Author		:	Jamie Taylor
	Last Edit	:	02/11/13
	Description	:	Demonstrates the performance gains custom allocators can
					bring to an application/code-base.

===============================================================================
*/


#include "MSC_Common.h"
#include "MSC_HeapAllocator.h"
#include "MSC_DynamicPoolAllocator.h"
#include <time.h>


// tweak to experiement
#define ALLOC_COUNT				128000
#define DYNAMIC_POOL_START_SIZE 1


// basic class for testing
class TestClass {
public:
	TestClass(  ) { /*std::cout << "TestClass( )" << std::endl;*/; }
	TestClass( const TestClass & ) { /*std::cout << "TestClass( & )" << std::endl;*/; }

	~TestClass( void ) { /*std::cout << "~TestClass( )" << std::endl;*/ }
private:
	// ...
};


// Basic allocator useage demonstration and timed comparison.
I32 main( I32 argc, const I8 *argv[] ) {
	UNREFERENCED_PARAM( argc );
	UNREFERENCED_PARAM( argv );

	TestClass copyCtorDemo;

	// ------------------------------------- Heap allocator testing -------------------------------------
	HeapAllocator heapAllocator;

	// Allocate and construct
	TestClass *p = reinterpret_cast<TestClass*>( heapAllocator.Allocate( sizeof( TestClass ) ) );
	heapAllocator.Construct<TestClass>( p );

	// Destruct and free
	heapAllocator.Destruct<TestClass>( p );
	heapAllocator.DeAllocate( p );
	p = NULL;
	// ------------------------------------- Heap allocator testing -------------------------------------

	// ---------------------------------- Dynamic Pool allocator testing --------------------------------
	DynamicPoolAllocator dynamicPoolAllocator( sizeof( TestClass ), DYNAMIC_POOL_START_SIZE );

	// Allocate and construct
	p = reinterpret_cast<TestClass*>( dynamicPoolAllocator.Allocate( ) );
	dynamicPoolAllocator.Construct( p, copyCtorDemo );

	// Destruct and free
	dynamicPoolAllocator.Destruct( p );
	dynamicPoolAllocator.DeAllocate( p );
	p = NULL;
	// ---------------------------------- Dynamic Pool allocator testing --------------------------------

	// --------------------------------- Custom allocators VS new & delete ------------------------------
	std::cout << "new & delete VS Dynamic Pool Allocator timed performance comparison:\n---" << std::endl;
	clock_t startTime = clock( );
	
	// new & delete
	for( I32 i=0; i<ALLOC_COUNT; ++i ) {
		p = new TestClass;
		delete p;
	}

	clock_t endTime = clock( );
	I64 timeDelta = static_cast<I64>( ( ( endTime - startTime ) / ( CLOCKS_PER_SEC / 1000.0f ) ) );

	std::cout << "Time taken for new & delete to allocate and construct: " << 
		ALLOC_COUNT << " instances: " << timeDelta << " ms" << std::endl;

	// heap
	startTime = clock( );
	for( I32 i=0; i<ALLOC_COUNT; ++i ) {
		p = reinterpret_cast<TestClass*>( heapAllocator.Allocate( sizeof( TestClass ) ) );
		heapAllocator.Construct<TestClass>( p );

		heapAllocator.Destruct<TestClass>( p );
		heapAllocator.DeAllocate( p );
	}

	endTime = clock( );
	timeDelta = static_cast<I64>( ( ( endTime - startTime ) / ( CLOCKS_PER_SEC / 1000.0f ) ) );

	std::cout << "Time taken for heap allocator to allocate and construct: " << 
		ALLOC_COUNT << " instances: " << timeDelta << " ms" << std::endl;


	// dynamic pool
	startTime = clock( );
	for( I32 i=0; i<ALLOC_COUNT; ++i ) {
		p = reinterpret_cast<TestClass*>( dynamicPoolAllocator.Allocate( ) );
		dynamicPoolAllocator.Construct<TestClass>( p );

		dynamicPoolAllocator.Destruct<TestClass>( p );
		dynamicPoolAllocator.DeAllocate( p );
	}

	endTime = clock( );
	timeDelta = static_cast<I64>( ( ( endTime - startTime ) / ( CLOCKS_PER_SEC / 1000.0f ) ) );

	std::cout << "Time taken for dynamic pool to allocate and construct: " << 
		ALLOC_COUNT << " instances: " << timeDelta << " ms" << std::endl;
	// --------------------------------- Custom allocators VS new & delete ------------------------------

	std::cin.get( );
	return 0;
}
