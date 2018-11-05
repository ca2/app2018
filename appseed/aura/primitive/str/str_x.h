#pragma once


namespace str
{


   CLASS_DECL_AURA bool x_contains(string str, string strFind);
   CLASS_DECL_AURA bool x_find(stringa & stra, string str);
   CLASS_DECL_AURA void x_left_remove(string & str, string strFind);

   extern CLASS_DECL_AURA const char g_trailingBytesForUTF8[256];


} // namespace str


inline int trailingBytesForUTF8(char ch)
{

   return ::str::g_trailingBytesForUTF8[(byte)ch];

}



