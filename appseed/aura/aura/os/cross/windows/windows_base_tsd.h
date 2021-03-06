/*
 * Compilers that uses ILP32, LP64 or P64 type models
 * for both Win32 and Win64 are supported by this file.
 *
 * Copyright (C) 1999 Patrik Stridvall
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#pragma once

#define _BASETSD_H_

#ifdef __cplusplus
extern "C" {
#endif /* defined(__cplusplus) */

/*
 * Win32 was easy to implement under Unix since most (all?) 32-bit
 * Unices uses the same type model (ILP32) as Win32, where int32_t, long
 * and pointer are 32-bit.
 *
 * Win64, however, will cause some problems when implemented under Unix.
 * Linux/{Alpha, Sparc64} and most (all?) other 64-bit Unices use
 * the LP64 type model where int32_t is 32-bit and long and pointer are
 * 64-bit. Win64 on the other hand uses the P64 (sometimes called LLP64)
 * type model where int32_t and long are 32 bit and pointer is 64-bit.
 */

#if defined(__x86_64__) && !defined(OS64BIT)
#define OS64BIT
#endif

/* Type model independent typedefs */
/* The __intXX types are native types defined by the MS C compiler.
 * Apps that make use of them before they get defined here, can
 * simply add to the command line:
 *    -D__int8=char -D__int16=short -D__int32=int32_t "-D__int64=long long"
 */
#if !defined(_MSC_VER) && !defined(__WIDL__)
#  ifndef __int8
#    define __int8  char
#  endif
#  ifndef __int16
#    define __int16 short
#  endif
#  ifndef __int32
#    define __int32 int32_t
#  endif
#  ifndef __int64
#    if defined(OS64BIT) && !defined(__MINGW64__)
#      define __int64 long
#    else
#      define __int64 long long
#    endif
#  endif
#endif /* !defined(_MSC_VER) */

/* FIXME: DECLSPEC_ALIGN should be declared only in winnt.h, but we need it here too */
#ifndef DECLSPEC_ALIGN
# if defined(_MSC_VER) && (_MSC_VER >= 1300) && !defined(MIDL_PASS)
#  define DECLSPEC_ALIGN(x) __declspec(align(x))
# elif defined(__GNUC__)
#  define DECLSPEC_ALIGN(x) __attribute__((aligned(x)))
# else
#  define DECLSPEC_ALIGN(x)
# endif
#endif

typedef signed char      INT8, *PINT8;
typedef signed short     INT16, *PINT16;
#ifndef RASPBIAN
typedef int32_t       INT32, *PINT32;
#endif
typedef unsigned char    UINT8, *PUINT8;
typedef unsigned short   UINT16, *PUINT16;
typedef uint32_t     UINT32, *PUINT32;
typedef int32_t       LONG32, *PLONG32;
typedef uint32_t     ULONG32, *PULONG32;
typedef uint32_t     DWORD32, *PDWORD32;

#ifdef _MSC_VER
typedef signed __int64   INT64, *PINT64;
typedef unsigned __int64 UINT64, *PUINT64;
typedef signed __int64   LONG64, *PLONG64;
typedef unsigned __int64 ULONG64, *PULONG64;
typedef unsigned __int64 DWORD64, *PDWORD64;
#else
typedef signed __int64   DECLSPEC_ALIGN(8) INT64, *PINT64;
typedef unsigned __int64 DECLSPEC_ALIGN(8) UINT64, *PUINT64;
typedef signed __int64   DECLSPEC_ALIGN(8) LONG64, *PLONG64;
typedef unsigned __int64 DECLSPEC_ALIGN(8) ULONG64, *PULONG64;
typedef unsigned __int64 DECLSPEC_ALIGN(8) DWORD64, *PDWORD64;
#endif
/* Basic pointer-sized integer types */

#if defined(__midl) || defined(__WIDL__)

typedef /* [public] */ signed __int3264   INT_PTR, *PINT_PTR;
typedef /* [public] */ signed __int3264   LONG_PTR, *PLONG_PTR;
typedef /* [public] */ unsigned __int3264 UINT_PTR, *PUINT_PTR;
typedef /* [public] */ unsigned __int3264 ULONG_PTR, *PULONG_PTR;
typedef ULONG_PTR                   DWORD_PTR, *PDWORD_PTR;

#elif defined(OS64BIT)

#define __int3264 __int64

typedef signed __int64   INT_PTR, *PINT_PTR;
typedef signed __int64   LONG_PTR, *PLONG_PTR;
typedef unsigned __int64 UINT_PTR, *PUINT_PTR;
typedef unsigned __int64 ULONG_PTR, *PULONG_PTR;
typedef ULONG_PTR        DWORD_PTR, *PDWORD_PTR;

#else
#if !defined(APPLE_IOS) && !defined(__int3264)
#define __int3264 __int32
#endif

#ifndef METROWIN

typedef int             INT_PTR, *PINT_PTR;
typedef unsigned int    UINT_PTR, *PUINT_PTR;
typedef int             LONG_PTR, *PLONG_PTR;
typedef unsigned int    ULONG_PTR, *PULONG_PTR;
typedef ULONG_PTR       DWORD_PTR, *PDWORD_PTR;

#endif

#endif


#ifdef APPLEOS

#if defined(OS64BIT)

   typedef int64_t int_ptr;
   typedef uint64_t uint_ptr;

#else

   typedef int32_t int_ptr;
   typedef uint32_t uint_ptr;

#endif

#else

   typedef INT_PTR int_ptr;
   typedef UINT_PTR uint_ptr;

#endif


//typedef LONG_PTR long_ptr;
//typedef ULONG_PTR ulong_ptr;
//typedef DWORD_PTR dword_ptr;

/* Win32 or Win64 dependent typedef/defines. */

#ifdef OS64BIT

#define MAXINT_PTR 0x7fffffffffffffff
#define MININT_PTR 0x8000000000000000
#define MAXUINT_PTR 0xffffffffffffffff

typedef __int64 SHANDLE_PTR;
typedef unsigned __int64 HANDLE_PTR;
typedef int32_t HALF_PTR, *PHALF_PTR;
typedef uint32_t UHALF_PTR, *PUHALF_PTR;

#define MAXHALF_PTR 0x7fffffff
#define MINHALF_PTR 0x80000000
#define MAXUHALF_PTR 0xffffffff

   /*
#if !defined(__midl) && !defined(__WIDL__)

inline ULONG_PTR HandleToULong(const void *h)
{
    return (ULONG_PTR)h;
}

inline LONG_PTR HandleToLong(const void *h)
{
    return (LONG_PTR)h;
}

inline void *ULongToHandle(ULONG32 ul)
{
    return (void *)(ULONG_PTR)ul;
}

inline void *LongToHandle(LONG32 l)
{
    return (void *)(LONG_PTR)l;
}

inline ULONG_PTR PtrToUlong(const void *p)
{
    return (ULONG_PTR)p;
}

inline LONG_PTR PtrToLong(const void *p)
{
    return (LONG_PTR)p;
}

inline UINT_PTR PtrToUint(const void *p)
{
    return (UINT_PTR)p;
}

inline INT_PTR PtrToInt(const void *p)
{
    return (INT_PTR)p;
}

inline ULONG_PTR PtrToUshort(const void *p)
{
    return (ULONG_PTR)p;
}

inline LONG_PTR PtrToShort(const void *p)
{
    return (LONG_PTR)p;
}

inline void *IntToPtr(INT32 i)
{
    return (void *)(INT_PTR)i;
}

inline void *UIntToPtr(UINT32 ui)
{
    return (void *)(UINT_PTR)ui;
}

inline void *LongToPtr(LONG32 l)
{
    return (void *)(LONG_PTR)l;
}

inline void *ULongToPtr(ULONG32 ul)
{
    return (void *)(ULONG_PTR)ul;
}

#endif

   */

#define HandleToULong(h)        ((WINULONG)(ULONG_PTR)(h))
#define HandleToLong(h)         ((LONG)(LONG_PTR)(h))
#define ULongToHandle(ul)       ((HANDLE)(ULONG_PTR)(ul))
#define LongToHandle(l)         ((HANDLE)(LONG_PTR)(l))
#define PtrToUlong(p)           ((WINULONG)(ULONG_PTR)(p))
#define PtrToLong(p)            ((LONG)(LONG_PTR)(p))
#define PtrToUint(p)            ((UINT)(UINT_PTR)(p))
#define PtrToInt(p)             ((INT)(INT_PTR)(p))
#define PtrToUshort(p)          ((WINUSHORT)(ULONG_PTR)(p))
#define PtrToShort(p)           ((SHORT)(LONG_PTR)(p))
#define IntToPtr(i)             ((void *)(INT_PTR)((INT)i))
#define UIntToPtr(ui)           ((void *)(UINT_PTR)((UINT)ui))
#define LongToPtr(l)            ((void *)(LONG_PTR)((LONG)l))
#define ULongToPtr(ul)          ((void *)(ULONG_PTR)((WINULONG)ul))

#else /* FIXME: defined(_WIN32) */

#ifndef METROWIN
#define MAXINT_PTR 0x7fffffff
#define MININT_PTR 0x80000000
#define MAXUINT_PTR 0xffffffff
#endif

typedef long SHANDLE_PTR;
typedef unsigned long HANDLE_PTR;
typedef signed short HALF_PTR, *PHALF_PTR;
typedef unsigned short UHALF_PTR, *PUHALF_PTR;

#ifndef METROWIN
#define MAXUHALF_PTR 0xffff
#define MAXHALF_PTR 0x7fff
#define MINHALF_PTR 0x8000
#endif

#ifndef METROWIN
#define HandleToULong(h)        ((WINULONG)(ULONG_PTR)(h))
#define HandleToLong(h)         ((LONG)(LONG_PTR)(h))
#define ULongToHandle(ul)       ((HANDLE)(ULONG_PTR)(ul))
#define LongToHandle(l)         ((HANDLE)(LONG_PTR)(l))
#define PtrToUlong(p)           ((WINULONG)(ULONG_PTR)(p))
#define PtrToLong(p)            ((LONG)(LONG_PTR)(p))
#define PtrToUint(p)            ((UINT)(UINT_PTR)(p))
#define PtrToInt(p)             ((INT)(INT_PTR)(p))
#define PtrToUshort(p)          ((WINUSHORT)(ULONG_PTR)(p))
#define PtrToShort(p)           ((SHORT)(LONG_PTR)(p))
#define IntToPtr(i)             ((void *)(INT_PTR)((INT)i))
#define UIntToPtr(ui)           ((void *)(UINT_PTR)((UINT)ui))
#define LongToPtr(l)            ((void *)(LONG_PTR)((LONG)l))
#define ULongToPtr(ul)          ((void *)(ULONG_PTR)((WINULONG)ul))
#endif

#endif /* defined(OS64BIT) || defined(_WIN32) */

#define HandleToUlong(h)        HandleToULong(h)
#define UlongToHandle(ul)       ULongToHandle(ul)
#define UintToPtr(ui)           UIntToPtr(ui)
#define UlongToPtr(ul)          ULongToPtr(ul)

typedef LONG_PTR SSIZE_T, *PSSIZE_T;
typedef ULONG_PTR SIZE_T, *PSIZE_T;

typedef ULONG_PTR KAFFINITY, *PKAFFINITY;

/* Some Wine-specific definitions */

/* Architecture dependent settings. */
/* These are hardcoded to avoid dependencies on config.h in Winelib apps. */
#if defined(__i386__) || defined(_WIN32)
# undef  WORDS_BIGENDIAN
# undef  BITFIELDS_BIGENDIAN
# define ALLOW_UNALIGNED_ACCESS
#elif defined(__x86_64__)
# undef  WORDS_BIGENDIAN
# undef  BITFIELDS_BIGENDIAN
# define ALLOW_UNALIGNED_ACCESS
#elif defined(__sparc__)
# define WORDS_BIGENDIAN
# define BITFIELDS_BIGENDIAN
# undef  ALLOW_UNALIGNED_ACCESS
#elif defined(__powerpc__)
# define WORDS_BIGENDIAN
# define BITFIELDS_BIGENDIAN
# undef  ALLOW_UNALIGNED_ACCESS
#elif defined(__ALPHA__)
# undef  WORDS_BIGENDIAN
# undef  BITFIELDS_BIGENDIAN
# undef  ALLOW_UNALIGNED_ACCESS
#elif defined(__ARMEB__)
# define WORDS_BIGENDIAN
# define BITFIELDS_BIGENDIAN
# undef  ALLOW_UNALIGNED_ACCESS
#elif defined(__ARMEL__)
# undef  WORDS_BIGENDIAN
# undef  BITFIELDS_BIGENDIAN
# undef  ALLOW_UNALIGNED_ACCESS
#elif defined(__MIPSEB__)
# define WORDS_BIGENDIAN
# define BITFIELDS_BIGENDIAN
# undef  ALLOW_UNALIGNED_ACCESS
#elif defined(__MIPSEL__)
# undef  WORDS_BIGENDIAN
# undef  BITFIELDS_BIGENDIAN
# undef  ALLOW_UNALIGNED_ACCESS
#elif defined(__APPLE__)
# undef  WORDS_BIGENDIAN
# undef  BITFIELDS_BIGENDIAN
# undef  ALLOW_UNALIGNED_ACCESS
#elif !defined(RC_INVOKED) && !defined(__WIDL__) && !defined(__midl)
# error Unknown CPU architecture!
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif /* defined(__cplusplus) */

