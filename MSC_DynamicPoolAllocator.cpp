/*
===============================================================================

	Sample Code for MSc Application
	===============================
	File		:	MSC_DynamicPoolAllocator.h
	Author		:	Jamie Taylor
	Last Edit	:	02/11/13
	Description	:	Dynamic pool memory allocator, will use pre-allocated memory if available
					otherwise it will request more memory from the system. Advantage over standard
					allocation comes from the fact that we retain memory, this allows subsequent
					allocations to use this pre-allocated memory and not incur another context switch.

===============================================================================
*/


#include "MSC_DynamicPoolAllocator.h"


/*
================
DynamicPoolAllocator::DynamicPoolAllocator
================
*/
DynamicPoolAllocator::DynamicPoolAllocator( U32 _elementSize, U32 numberOf, U8 _alignment ) {
	//RT_SLOW_ASSERT(!(_alignment < 1)); // used to avoid having user specify isAligned when freeing

	elementSize = ( _elementSize > sizeof( PoolElement ) ) ? _elementSize : sizeof( PoolElement );
	alignment = _alignment;

	// setup x initial elements - starting with the head
	head  = reinterpret_cast<PoolElement*>( Alloc( ) ); 
	head->next = NULL;

	// additional elements, if numberOf > 1
	for( U32 i=0; i<( numberOf - 1 ); ++i ) {
		PoolElement *temp = reinterpret_cast<PoolElement*>( Alloc( ) );
		temp->next = head;
		head = temp;
	}
}

/*
================
DynamicPoolAllocator::~DynamicPoolAllocator
================
*/
DynamicPoolAllocator::~DynamicPoolAllocator( void ) {
	Clear( );
}

/*
================
DynamicPoolAllocator::Allocate

Get an element from the pool; exposed to user
================
*/
void* DynamicPoolAllocator::Allocate( void ) {
	// static pool version would assert this to ensure we're not out of memory 
	if( head != NULL ) {
		PoolElement *temp = head;
	    head = head->next;
		return reinterpret_cast<void*>( temp );
	} else {
	    return Alloc( );
	}
}

/*
================
DynamicPoolAllocator::Alloc

Aligned and unaligned allocation; not exposed to user
================
*/
void* DynamicPoolAllocator::Alloc( void ) {
	// dynamically allocate memory for another element
	size_t  temp = reinterpret_cast<size_t>( operator new( elementSize + alignment ) ); 

	// get the misalignment - mask = alignment-1
	size_t misalignment = ( temp & ( alignment-1 ) );
	// get the adjustment needed
	size_t adjustment = alignment - misalignment;
	// get the newly aligned address
	temp += adjustment;

	// store adjustment info in preceeding byte
	uintptr_t *p = reinterpret_cast<uintptr_t*>( temp - 1 );
	*p = adjustment;

	return reinterpret_cast<void*>( temp );
}

/*
================
DynamicPoolAllocator::DeAllocate

Hand back a pool element; exposed to user
================
*/
void DynamicPoolAllocator::DeAllocate( void *putBack ) {
	PoolElement *temp = reinterpret_cast<PoolElement*>( putBack );
	temp->next = head;
	head = temp;
}

/*
================
DynamicPoolAllocator::Clear

Release all memory/pool elements - including head; not exposed to user
================
*/
void DynamicPoolAllocator::Clear( void ) {
	U8 *toFree = NULL;
	PoolElement *temp = NULL;
	while( head != NULL ) {
	    temp = head;
	    head = head->next;
 	    toFree = reinterpret_cast<U8*>( temp );
	    toFree = ( toFree - ( *( toFree-1 ) ) );

	    operator delete( reinterpret_cast<void*>( toFree ) );
	}
	head = NULL;
}