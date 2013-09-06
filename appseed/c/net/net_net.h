#pragma once



CLASS_DECL_c int_bool from_string(in6_addr * addr, const char * string);
CLASS_DECL_c string to_vsstring(const in6_addr *addr);
CLASS_DECL_c int_bool from_string(in_addr * addr, const char * string);
CLASS_DECL_c string to_vsstring(const in_addr *addr);

CLASS_DECL_c uint32_t c_inet_addr(const char *src);
CLASS_DECL_c int32_t c_inet_pton(int32_t af, const char *src, void *dst);
CLASS_DECL_c const char * c_inet_ntop(int32_t af, const void *src, char *dst, int32_t cnt);
CLASS_DECL_c string c_inet_ntop(int32_t af, const void *src);


CLASS_DECL_c string c_gethostbyname(const char * hostname);

#if !defined(MACOS) && !defined(ANDROID)

#if BYTE_ORDER == BIG_ENDIAN

#define HTONS(n) (n)
#define NTOHS(n) (n)
#define HTONL(n) (n)
#define NTOHL(n) (n)

#else

#define HTONS(n) (((((uint16_t)(n) & 0xFF)) << 8) | (((uint16_t)(n) & 0xFF00) >> 8))
#define NTOHS(n) (((((uint16_t)(n) & 0xFF)) << 8) | (((uint16_t)(n) & 0xFF00) >> 8))

#define HTONL(n) (((((unsigned long)(n) & 0xFF)) << 24) | \
                  ((((unsigned long)(n) & 0xFF00)) << 8) | \
                  ((((unsigned long)(n) & 0xFF0000)) >> 8) | \
                  ((((unsigned long)(n) & 0xFF000000)) >> 24))

#define NTOHL(n) (((((unsigned long)(n) & 0xFF)) << 24) | \
                  ((((unsigned long)(n) & 0xFF00)) << 8) | \
                  ((((unsigned long)(n) & 0xFF0000)) >> 8) | \
                  ((((unsigned long)(n) & 0xFF000000)) >> 24))
#endif

#endif

#if 0

uint16_t htons(uint16_t n);
uint16_t ntohs(uint16_t n);
unsigned long htonl(unsigned long n);
unsigned long ntohl(unsigned long n);

#define htons(n) HTONS(n)
#define ntohs(n) NTOHS(n)

#define htonl(n) HTONL(n)
#define ntohl(n) NTOHL(n)

#endif

