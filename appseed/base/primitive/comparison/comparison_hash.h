#pragma once






template < class ARG_KEY >
inline UINT HashKey(ARG_KEY key)
{
   // default identity hash - works for most primitive values
   return (uint32_t)(((uint_ptr)key)>>4);
}


namespace comparison
{

   template < class ARG_KEY >
   class hash
   {
   public:

      inline static UINT HashKey(ARG_KEY key)
      {
         return ::HashKey < ARG_KEY > (key);
      }

   };

   template < >
   class CLASS_DECL_BASE hash < const string & >
   {
   public:

      inline static UINT HashKey (const string & key)
      {
         register uint64_t * puiKey = (uint64_t *) (const char *) key;
         register strsize counter = key.get_length();
         register uint64_t nHash = 0;
         while(counter >= sizeof(*puiKey))
         {
            nHash = (nHash<<5) + nHash + *puiKey++;
            counter -= sizeof(*puiKey);
         }
         register const char * pszKey = (const char *) puiKey;
         while(counter-- >= 0) nHash = (nHash<<5) + nHash + *pszKey++;
         return (UINT) (nHash & 0xffffffff);
      }

   };

   template < >
   class CLASS_DECL_BASE hash < const wstring & >
   {
   public:

      inline static UINT HashKey (const wstring & key)
      {
         register uint64_t * puiKey = (uint64_t *) (const wchar_t *) key;
         register strsize counter = key.get_length() * sizeof(wchar_t);
         register uint64_t nHash = 0;
         while(counter >= sizeof(*puiKey))
         {
            nHash = (nHash<<5) + nHash + *puiKey++;
            counter -= sizeof(*puiKey);
         }
         register const wchar_t * pszKey = (const wchar_t *) puiKey;
         while(true)
         {
            counter -= 2;
            if(counter < 0)
               break;
               nHash = (nHash<<5) + nHash + *pszKey++;
         }
         return (UINT)(nHash & 0xffffffff);
      }

   };
   class CLASS_DECL_BASE strid_hash
   {
   public:

      inline static UINT HashKey (const id & key)
      {
         return (UINT) (*((int_ptr*)(&key.m_pstr)) >> 8);
      }

   };
   
   
   template < >
   class CLASS_DECL_BASE hash < id >
   {
   public:
      
      inline static UINT HashKey (const id & key)
      {
         return strid_hash::HashKey(key);
      }
      
   };


} // namespace comparison



template<> CLASS_DECL_BASE UINT HashKey<const wchar_t *> (const wchar_t * key);


template<> inline UINT HashKey<rect> (rect key)
{
   return key.left | key.top | key.bottom | key.right;
}

template<> inline UINT HashKey<point> (point key)
{
   return key.x | key.y;
}

template<> inline UINT HashKey<size> (size key)
{
   return key.cx | key.cy;
}

template<> inline UINT HashKey<const char *> (const char * key)
{
   register uint64_t * puiKey = (uint64_t *) key;
   register strsize counter = strlen(key);
   register uint64_t nHash = 0;
   while(counter >= sizeof(*puiKey))
   {
      nHash = (nHash<<5) + nHash + *puiKey++;
      counter -= sizeof(*puiKey);
   }
   register const char * pszKey = (const char *) puiKey;
   while(counter-- >= 0) nHash = (nHash<<5) + nHash + *pszKey++;
   return (UINT) nHash;
}


template <> inline UINT HashKey(const id & id)
{
   // default identity hash - works for most primitive values
   return ::comparison::strid_hash::HashKey(id);
}
