#pragma once


typedef uint8_t            byte;
typedef unsigned char      uchar;
typedef uint32_t           dword;


#ifdef __cplusplus
template < typename TARGET, typename SOURCE >
TARGET throw_cast(SOURCE src)
{
   return (TARGET) src;
}
#endif





