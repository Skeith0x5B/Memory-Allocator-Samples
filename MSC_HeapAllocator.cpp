/*
===============================================================================

    Sample Code for MSc Application
    ===============================
    File        :   MSC_HeapAllocator.h
    Author      :   Jamie Taylor
    Last Edit   :   02/11/13
    Description :   Basic heap allocator - uses memory from the heap and thus shouldn't be heavily used.

                    Custom allocators are used since they allow complete control over allocation&construction via
                    operator and placement new and destruction&deallocation via operator delete. The custom allocators
                    also support aligned allocations and allocation logging/tracking.

                    Pre-allocated buffers should be preferred where possible. 

===============================================================================
*/


#include "MSC_HeapAllocator.h"

/*
================
HeapAllocator::HeapAllocator
================
*/
HeapAllocator::HeapAllocator( void ) {
    // ...
}

/*
================
HeapAllocator::~HeapAllocator
================
*/
HeapAllocator::~HeapAllocator( void ) {
    // ...
}

/*
================
HeapAllocator::Allocate
================
*/
void* HeapAllocator::Allocate( size_t sizeInBytes, U8 alignment ) {
    //RT_SLOW_ASSERT( ( alignment < 1 ) == false );
    size_t temp = reinterpret_cast<size_t>( operator new( ( sizeInBytes + alignment ) ) );

    // get the misalignment, (alignment-1 = mask)
    U8 misalignment = ( temp & ( alignment-1 ) );
    // get the adjustment needed
    U8 adjustment = alignment - misalignment;
    // get the newly aligned address
    temp += adjustment;

    // store adjustment information in the byte immediately preceding adjusted address 
    // using(02/11/13: uintptr_t here was causing heap corruption, haven't seen this issue before)
    U8 *p = reinterpret_cast<U8*>( temp-1 );
    *p = adjustment;

    return reinterpret_cast<void*>( temp );
}

/*
================
HeapAllocator<T>::DeAllocate
================
*/
void HeapAllocator::DeAllocate( void *freeThis ) {
    // get the alignment info and adjust if needed
    U8 *temp = reinterpret_cast<U8*>( freeThis );
    temp = ( temp - ( *( temp-1 ) ) );

    operator delete( reinterpret_cast<void*>( temp ) );
}

