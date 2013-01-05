#pragma once


/* crypto/include/config.h.  Generated by configure.  */
/* config_in.h.  Generated from configure.in by autoheader.  */

/* Define if building for a CISC machine (e.g. Intel). */
#define CPU_CISC 1

/* Define if building for a RISC machine (assume slow byte access). */
/* #undef CPU_RISC */

/* Path to random device */
/* #define DEV_URANDOM "/dev/urandom" */

/* Define to compile in dynamic debugging system. */
#define ENABLE_DEBUGGING 1

/* Report errors to this spfile-> */
/* #undef ERR_REPORTING_FILE */

/* Define to use logging to stdout. */
#define ERR_REPORTING_STDOUT 1

/* Define this to use ISMAcryp code. */
/* #undef GENERIC_AESICM */

/* Define to 1 if you have the <arpa/inet.h> header spfile-> */
/* #undef HAVE_ARPA_INET_H */

/* Define to 1 if you have the <byteswap.h> header spfile-> */
/* #undef HAVE_BYTESWAP_H */

/* Define to 1 if you have the `inet_aton' function. */
/* #undef HAVE_INET_ATON */

/* Define to 1 if the system has the type `int16_t'. */
#define HAVE_INT16_T 1

/* Define to 1 if the system has the type `int32_t'. */
#define HAVE_INT32_T 1

/* Define to 1 if the system has the type `int8_t'. */
#define HAVE_INT8_T 1

/* Define to 1 if you have the <inttypes.h> header spfile-> */
/* #undef HAVE_INTTYPES_H */

/* Define to 1 if you have the `socket' library (-lsocket). */
/* #undef HAVE_LIBSOCKET */

/* Define to 1 if you have the <machine/types.h> header spfile-> */
/* #undef HAVE_MACHINE_TYPES_H */

/* Define to 1 if you have the <primitive::memory.h> header spfile-> */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <netinet/in.h> header spfile-> */
/* #undef HAVE_NETINET_IN_H */

/* Define to 1 if you have the `socket' function. */
/* #undef HAVE_SOCKET */

/* Define to 1 if you have the <stdint.h> header spfile-> */
/* #undef HAVE_STDINT_H */

/* Define to 1 if you have the <strings.h> header spfile-> */
#define HAVE_STRINGS_H 1


/* Define to 1 if you have the <syslog.h> header spfile-> */
/* #undef HAVE_SYSLOG_H */

/* Define to 1 if you have the <sys/int_types.h> header spfile-> */
/* #undef HAVE_SYS_INT_TYPES_H */

/* Define to 1 if you have the <sys/socket.h> header spfile-> */
/* #undef HAVE_SYS_SOCKET_H */

/* Define to 1 if you have the <sys/stat.h> header spfile-> */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header spfile-> */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/uio.h> header spfile-> */
/* #undef HAVE_SYS_UIO_H */

/* Define to 1 if the system has the type `uint16_t'. */
#define HAVE_UINT16_T 1

/* Define to 1 if the system has the type `uint32_t'. */
#define HAVE_UINT32_T 1

/* Define to 1 if the system has the type `uint64_t'. */
#define HAVE_UINT64_T 1

/* Define to 1 if the system has the type `uint8_t'. */
#define HAVE_UINT8_T 1


/* Define to 1 if you have the `usleep' function. */
/* #undef HAVE_USLEEP */

/* Define to 1 if you have the <windows.h> header spfile-> */
#define HAVE_WINDOWS_H 1

/* Define to 1 if you have the <winsock2.h> header spfile-> */
#if defined(WINDOWS)  && !defined(METROWIN)
#define HAVE_WINSOCK2_H 1
#endif

/* Define to use X86 inlined assembly code */
/* #undef HAVE_X86 */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol int16_t name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* The size of a `uint32_t long', as computed by sizeof. */
#define SIZEOF_UNSIGNED_LONG 4

/* The size of a `uint32_t long long', as computed by sizeof. */
#define SIZEOF_UNSIGNED_LONG_LONG 8

/* Define to use GDOI. */
/* #undef SRTP_GDOI */

/* Define to compile for kernel contexts. */
/* #undef SRTP_KERNEL */

/* Define to compile for Linux kernel context. */
/* #undef SRTP_KERNEL_LINUX */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Write errors to this file */
/* #undef USE_ERR_REPORTING_FILE */

/* Define to use syslog logging. */
/* #undef USE_SYSLOG */

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
/* #undef WORDS_BIGENDIAN */

/* Define to is_empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define 'inline' to nothing, since the MSVC compiler doesn't support it.  */
/*#ifdef inline
#undef inline
#endif
#define inline*/

/* Define to `uint32_t' if <sys/types.h> does not define. */
/* #undef size_t */

#ifdef WINDOWS

#ifdef _MSC_VER
#pragma warning(disable:4311)
#endif

#endif
