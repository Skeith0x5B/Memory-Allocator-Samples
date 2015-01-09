/*
===============================================================================

	Sample Code for MSc Application
	===============================
	File		:	MSC_HeapAllocator.h
	Author		:	Jamie Taylor
	Last Edit	:	02/11/13
	Description	:	Basic heap allocator - uses memory from the heap and thus shouldn't be heavily used.

					Custom allocators are used since they allow complete control over allocation&construction via
					operator and placement new and destruction&deallocation via operator delete. The custom allocators
					also support aligned allocations and allocation logging/tracking.

					Pre-allocated buffers should be preferred where possible. 

===============================================================================
*/

 
#ifndef MSC_HEAP_ALLOCATOR_H
#define MSC_HEAP_ALLOCATOR_H


#include "MSC_Common.h"
//#include "RtAssert.h"


/*
===============================================================================

Heap Allocator class

===============================================================================
*/
class HeapAllocator {
public:
			HeapAllocator( void );
			~HeapAllocator( void );

			// Allocate - get uninitialized memory
	void *	Allocate( size_t sizeInBytes, U8 alignment = 1 );

			// DeAllocate
	void   	DeAllocate( void *freeThis );

			// Construct & destruct- don't use on POD or void
			template<class T>
	void	Construct( T *posInMem );
			template<class T>
	void   	Construct( T *posInMem, const T &val );
			template<class T>
	void 	Destruct( T *toDestruct );

private:
	// ...
};


/*
================
HeapAllocator::Construct

The second version of this function (copy constructor) was added
when g++ threw up type errors, VS2010 however was perfectly fine
with "const T& = 0".

Templates must be declared and defined in the header.
Use placement new to construct the object at a specified location in memory.
================
*/
template<class T>
void HeapAllocator::Construct( T *posInMem ) {
    new( reinterpret_cast<void*>(  posInMem ) ) T( );
}

template<class T>
void HeapAllocator::Construct( T *posInMem, const T &val ) {
	new( reinterpret_cast<void*>( posInMem ) ) T( val );
}

/*
================
HeapAllocator::Destruct

Again, templates must be declared and defined in the header.
================
*/
template<class T>
void HeapAllocator::Destruct( T *toDestruct ) {
	toDestruct->~T( );
}

#endif // MSC_HEAP_ALLOCATOR_H
