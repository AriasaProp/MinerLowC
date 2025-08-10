/*
 *  Util Header
 *
 *  Provide basic function and constant for all source code
 *
 *
 *
 */

#ifndef COMMON_INCLUDED_
#define COMMON_INCLUDED_

#include <stddef.h>
#include <stdlib.h>

// ================================
//  Global Macro
// ================================

#if (defined(_MSC_VER) && _MSC_VER < 1600) /*|| defined(__SYMBIAN32__) */
typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
typedef unsigned __int64 size_t;
#else
#include <stdint.h>
#endif

#if defined(_MSC_VER)
#define INLINE        __forceinline
#define CDECL         __cdecl
#define UNUSED(x)     ((void)x)
#define UNUSED_ARG(x) __pragma(warning(suppress : 4100 4101)) x
#elif defined(__GNUC__) || defined(__clang__)
#define INLINE        inline
#define CDECL         /* no translate */
#define UNUSED(x)     ((void)x)
#define UNUSED_ARG(x) __attribute__((unused)) x
#else
#error Unknown compiler
#endif

#define MAX(X, Y)      ((X > Y) ? (X) : (Y))
#define MIN(X, Y)      ((X < Y) ? (X) : (Y))
#define CLAMP(X, Y, Z) ((X < Z) ? ((Y > Z) ? (Z) : (Y)) : (X))

#endif // COMMON_INCLUDED_