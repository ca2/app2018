#pragma once


#ifdef __cplusplus

namespace primitive
{

#if defined(__LP64__)  // X64


#define memory_size_t uint64_t
#define memory_position_t uint64_t
#define memory_offset_t int64_t 

#elif defined(_M_X64) // X64


//DEFINE_C_NUMBER(CLASS_DECL_AURA, memory_size_t         , uint64_t)
//DEFINE_C_NUMBER(CLASS_DECL_AURA, memory_position_t     , uint64_t)
//DEFINE_C_NUMBER(CLASS_DECL_AURA, memory_offset_t       ,  int64_t)

#define memory_size_t uint64_t
#define memory_position_t uint64_t
#define memory_offset_t int64_t 


#else

//DEFINE_C_NUMBER(CLASS_DECL_AURA,   memory_size_t         , uint32_t)
//DEFINE_C_NUMBER(CLASS_DECL_AURA,   memory_position_t     , uint32_t)
//DEFINE_C_NUMBER(CLASS_DECL_AURA,   memory_offset_t       ,  int32_t)

#define memory_size_t uint32_t
#define memory_position_t uint32_t
#define memory_offset_t int32_t 


#endif


} // namespace primitive


#endif



CLASS_DECL_AURA void * memset_dup(void * p, int32_t i, size_t iSize);
CLASS_DECL_AURA void * memand_dup(void * p, const void * p1, const void * p2, size_t iSize);
CLASS_DECL_AURA void * memcpy_dup(void * dst, const void * src, size_t iSize);
CLASS_DECL_AURA void * memmov_dup(void * dst, const void * src, size_t iSize);
CLASS_DECL_AURA int32_t  memcmp_dup(const void * sz1, const void * sz2, size_t iSize);
CLASS_DECL_AURA unichar * wmemcpy_dup(unichar * dst, const unichar * src, size_t iSize);

CLASS_DECL_AURA void * mem_reverse(void * p, size_t iSize);
CLASS_DECL_AURA void * memrcpy_dup(void * p, const void * p1, size_t iSize);

#ifdef __cplusplus

inline void * zero_memory_dup(void * p, size_t iSize) { memset(p, 0, iSize); return p; }
inline void * zeroe_dup(void * p, size_t iSize) { memset(p, 0, iSize); return p; }
inline void * zero_dup(void * p, size_t iSize) { memset(p, 0, iSize); return p; }
inline void * zero_memory(void * p, size_t iSize) { memset(p, 0, iSize); return p; }
inline void * zeroe(void * p, size_t iSize) { memset(p, 0, iSize); return p; }
inline void * zero(void * p, size_t iSize) { memset(p, 0, iSize); return p; }

inline bool is_zero(void * p, size_t iSize)
{
   byte * pb = (byte *) p;
   while(*pb == '\0' && iSize > 0)
   {
      pb++;
      iSize--;
   }
   return iSize == 0;
}


#define ZEROP(px) ::zero(px, sizeof(*px));
#define ZERO(x) ::zero(&x, sizeof(x));
#define IS_ZEROP(px) (::is_zero(px, sizeof(*px)))
#define IS_ZERO(x) (::is_zero(&x, sizeof(x)))
#define ISNT_ZEROP(px) (!IS_ZEROP(px))
#define ISNT_ZERO(x) (!IS_ZERO(x))


#endif






