# Memory-Allocator-Samples
A couple of custom memory allocators I wrote for my first game engine project, put into a project of their own as a small supplement to MSc applications.

---------
Platforms
---------
Compiled and tested on:  - W7 and VS2010 (warning level 4 & treat warnings as errors).
            - Ubuntu 12.04.

------------
MSC_Common.h
------------
Atomic data type declarations.


-----------------------
MSC_HeapAllocator.h/cpp
-----------------------
Basic example allocator, mostly demonstrates programming style and
the approach being taken. Performance gains here are minimal since we'll
still incur a context switch like we would with new and delte, the only
benefit lies in the fact that separating allocation and construction gives
us greater flexibility and control, especially useful in performance constrained
situations.


------------------------------
MSC_DynamicPoolAllocator.h/cpp
------------------------------
The advantages of custom allocators start to show here, this variant is
dynamic and will allocate more memory from the system when it needs to.
I have a variant that uses a pre-allocated buffer and once exhausted
will fail to allocate memory but only present one to keep things succinct.

------------
MSC_Main.cpp
------------
Basic program demonstrating the performance gains that can be achieved by
using custom allocators.
