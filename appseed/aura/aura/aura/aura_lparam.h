#pragma once

//class cnull
//{
//public: 
//};




class CLASS_DECL_AURA lparam
{
public:


   LPARAM m_lparam;

   lparam(LPARAM lparam = 0)
   {
      m_lparam = lparam;
   }


#if !defined(ANDROID) && !defined(SOLARIS) && !defined(APPLE_IOS)

   lparam(int32_t i)
   {
      m_lparam = (LPARAM) i;
   }

#endif

#if !defined(LINUX) && !(defined(OS64BIT) && defined(WINDOWS)) && !defined(APPLEOS)

   lparam(int64_t i)
   {
      m_lparam = (LPARAM) i;
   }

#endif
   
//#ifdef APPLEOS
//#ifdef OS64BIT
   
//   lparam(long l)
  // {
    //  m_lparam = (LPARAM) l;
   //}
   
//#endif
//#endif

   lparam(const void * p)
   {
      m_lparam = (LPARAM) p;
   }

   lparam(const lparam & lparam)
   {
      m_lparam = lparam.m_lparam;
   }

   operator LPARAM &()
   {
      return m_lparam;
   }

   template < typename T >
   operator T * () const
   {
      return (T *) m_lparam;
   }

   lparam & operator = (const lparam & lparam)
   {
      m_lparam = lparam.m_lparam;
      return *this;
   }

   lparam & operator = (LPARAM lparam)
   {
      m_lparam = lparam;
      return *this;
   }

   inline int x() const
   {
      return GET_X_LPARAM(m_lparam);
   }

   inline int y() const
   {
      return GET_Y_LPARAM(m_lparam);
   }

};
