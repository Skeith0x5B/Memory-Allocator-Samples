/*
===============================================================================

	Sample Code for MSc Application
	===============================
	File		:	MSC_Common.h
	Author		:	Jamie Taylor
	Last Edit	:	02/11/13
	Description	:	Basic atomic data types. 

===============================================================================
*/


#ifndef MSC_COMMON_H
#define MSC_COMMON_H


#define NULL 0


#include <cstdint>
#include <iostream>
#if defined ( _WIN32 )
        //#include <WinNT.h>
        #include <WTypes.h>
	#define UNREFERENCED_PARAM( x ) UNREFERENCED_PARAMETER( x )
#else
        #define UNREFERENCED_PARAM( x ) 0
#endif


// Unsigned integer types
typedef unsigned char	U8;
typedef unsigned short	U16;
typedef unsigned int	U32;

// Signed integer types
typedef char	I8;
typedef short	I16;
typedef int		I32;

// 64 bit integer (look for MSVC)
#if defined(_MSC_VER)
	typedef unsigned __int64 U64;
	typedef __int64 I64;
#else
	typedef unsigned long long U64;
	typedef long long I64;
#endif // (_MSC_VER)

// 32 and 64bit floating point
typedef float  F32;
typedef double F64;


#endif // MSC_COMMON_H
