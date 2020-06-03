/*
===============================================================================

    Sample Code for MSc Application
    ===============================
    File        :   MSC_DynamicPoolAllocator.h
    Author      :   Jamie Taylor
    Last Edit   :   02/11/13
    Description :   Dynamic pool memory allocator, will use pre-allocated memory if available
                    otherwise it will request more memory from the system. Advantage over standard
                    allocation comes from the fact that we retain memory, this allows subsequent
                    allocations to use this pre-allocated memory and not incur another context switch.

===============================================================================
*/


#ifndef MSC_DYNAMIC_POOL_ALLOCATOR_H
#define MSC_DYNAMIC_POOL_ALLOCATOR_H


//#include "../RtCommonHeaders.h"
#include "MSC_Common.h"


/*
===============================================================================

Dynamic Pool Allocator class

===============================================================================
*/
class DynamicPoolAllocator {
public:
            DynamicPoolAllocator( U32 _elementSize, U32 numberOf = 32, U8 _alignment = 1 );
            ~DynamicPoolAllocator( void );

            // Get and return pool elements 
    void *  Allocate( void );
    void    DeAllocate( void *putBack );

            // Construct & destruct- don't use on POD or void
            template<class T>
    void    Construct( T *posInMem );
            template<class T>
    void    Construct( T *posInMem, const T &val );
            template<class T>
    void    Destruct( T *toDestruct );

private:
            // Stays in 'linked list' when free, memory holds data when in use 
            // and is 'recycled' to hold address of next element when put back in pool
            struct PoolElement {
                PoolElement *next;
            };

            // Aligned and unaligned allocation - dynamically allocate memory for pool elements
    void *  Alloc( );
            // Hand back dynamically allocated memory
    void    Clear( );

            // alignment, need to store this, static pool allocator can simply align the buffer but dynamic allocation isn't necessarily contiguous so store it internally 
    U8      alignment;
            // element size, T may be smaller than the size of an int (memory address) such as the *next member of the PoolElement struct, so we'll store it ourselves - Added back 17.08
    U32     elementSize;
            // head of the free element 'linked list'
    PoolElement * head;
};


/*
================
DynamicPoolAllocator::Construct

The second version of this function (copy constructor) was added
when g++ threw up type errors, VS2010 however was perfectly fine
with "const T& = 0".

Templates must be declared and defined in the header.
Use placement new to construct the object at a specified location in memory.
================
*/
template<class T>
void DynamicPoolAllocator::Construct( T *posInMem ) {
    new( reinterpret_cast<void*>(  posInMem ) ) T( );
}

template<class T>
void DynamicPoolAllocator::Construct( T *posInMem, const T &val ) {
    new( reinterpret_cast<void*>( posInMem ) ) T( val );
}

/*
================
DynamicPoolAllocator::Destruct

Again, templates must be declared and defined in the header.
================
*/
template<class T>
void DynamicPoolAllocator::Destruct( T *toDestruct ) {
    toDestruct->~T( );
}


#endif // MSC_DYNAMIC_POOL_ALLOCTOR_H
