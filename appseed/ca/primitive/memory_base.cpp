#include "StdAfx.h"

namespace primitive
{

   memory_base::memory_base()
   {

      m_pmutex             = NULL;
      m_iOffset            = 0;
      m_bLockMode          = false;
      m_bLock              = false;
      m_pcontainer         = NULL;
      m_cbStorage          = 0;
      m_dwAllocation       = 0;
      m_dwAllocationAddUp  = 0;

   }



   memory_base::~memory_base()
   {
   }

   memory_size memory_base::get_size() const
   {
      return m_cbStorage;
   }

   LPBYTE memory_base::get_data() const
   {
      if(IsEnabled())
         return internal_get_data() + m_iOffset;
      else
      {
         ASSERT(FALSE);
         return NULL;
      }
   }

   LPBYTE memory_base::internal_get_data() const
   {
      return NULL;
   }

   bool memory_base::operator ==(memory_base& s)
   {
      bool b = false;
      single_lock sl(m_pmutex);
      sl.lock();
      if(this->get_size() == s.get_size())
      b = memcmp(get_data(), s.get_data(), (size_t) this->get_size()) == 0;
      sl.unlock();
      return b;
   }
   void memory_base::allocate_add_up(memory_size dwAddUp)
   {
      allocate(m_cbStorage + dwAddUp);
   }


   bool memory_base::allocate(memory_size dwNewLength)
   {
      if(!IsEnabled())
      {
         ASSERT(FALSE);
         return false;
      }

      if(dwNewLength <= 0)
      {
         m_iOffset = 0;
         m_cbStorage = 0;
         return true;
      }

      if(dwNewLength > m_dwAllocation)
      {
         allocate_internal(dwNewLength);
      }

      if(dwNewLength > m_dwAllocation)
         return false;

      m_cbStorage = dwNewLength;
      return true;
   }


   bool memory_base::allocate_internal(memory_size dwNewLength)
   {
      UNREFERENCED_PARAMETER(dwNewLength);
      return false;
   }

   void memory_base::FullLoad(ex1::file & file)
   {

      if(!IsEnabled())
      {
         ASSERT(false);
         return;
      }

      uint64_t dwTemp;
      memory_size cbStorage = (memory_size) file.get_length();
      file.seek_to_begin();
      allocate(cbStorage);
      try
      {
         dwTemp = file.read(get_data(), cbStorage);
      }
      catch(ex1::file_exception_sp * pe)
      {
         TRACE0("smfOpenFile: read error on image!");
   #ifdef _DEBUG
         (*pe)->dump(afxdump);
   #endif
         delete pe;
      }
      if (cbStorage != dwTemp)
      {
         TRACE0("smfOpenFile: read error on image!");
      }
   }

   bool memory_base::IsLocked() const
   {
      if(m_pmutex == NULL)
         return false;
      else
         return m_pmutex->is_locked();
   }

   bool memory_base::IsEnabled() const
   {
      if(m_bLockMode)
         return IsLocked();
      else
         return true;

   }

   void memory_base::copy_from(const memory_base *pstorage)
   {
      ASSERT(pstorage != NULL);
      allocate(pstorage->get_size());
      memcpy(get_data(), pstorage->get_data(), (size_t) this->get_size());
   }

   void memory_base::set_data(void *pdata, memory_size uiSize)
   {
      allocate(uiSize);
      memcpy(get_data(), pdata, (size_t) uiSize);
   }

   void memory_base::delete_begin(memory_size iSize)
   {
      ASSERT(iSize >= 0);
      ASSERT(iSize <= this->get_size());
      if(iSize >= 0 &&
         iSize <= this->get_size())
      {
         m_iOffset += iSize;
         if(m_pcontainer != NULL)
         {
            m_pcontainer->offset_kept_pointers((::primitive::memory_offset) iSize);
         }
         m_cbStorage -= iSize;
      }
   }

   void memory_base::eat_begin(void * pdata, memory_size iSize)
   {
      ASSERT(iSize >= 0);
      ASSERT(iSize <= this->get_size());
      if(iSize >= 0 &&
         iSize <= this->get_size())
      {
         memcpy(pdata, get_data(), (size_t) iSize);
         delete_begin(iSize);
      }
   }

   void memory_base::_RemoveOffset()
   {
      if(get_data() != NULL && m_cbStorage > (uint64_t) m_iOffset)
      {
         memmove(get_data(), get_data() + m_iOffset, (size_t) (m_cbStorage - m_iOffset));
      }
      m_iOffset = 0;
   }

   memory_base & memory_base::operator = (const memory_base & s)
   {
      if(this != &s)
      {
         copy_from(&s);
      }
      return *this;
   }




   void memory_base::To(string & str, memory_position dwStart, memory_position dwEnd)
   {
      dwStart = max(dwStart, 0);
      if(dwEnd == ((memory_position)(-1)))
         dwEnd = this->get_size() - 1;
      char * pch = (char *) get_data();
      for(uint64_t dw = dwStart; dw <= dwEnd; dw++)
      {
         if(((pch[dw] & 0xf0) >> 4) < 10)
            str += (char)(((pch[dw] & 0xf0) >> 4) + '0');
         else
            str += (char)(((pch[dw] & 0xf0) >> 4) + 'A' - 10);
         if(((pch[dw] & 0x0f)) < 10)
            str += (char)((pch[dw] & 0x0f) + '0');
         else
            str += (char)((pch[dw] & 0x0f) + 'A' - 10);
      }
   }

   void memory_base::From(const char * psz)
   {
      char ch;
      int iLen = strlen(psz);
      allocate(iLen / 2);
      char * pch = (char *) get_data();
      while(*psz != '\0')
      {
         ch = 0;
         if(*psz > '9')
            ch |= ((*psz - 'A' + 10) & 0x0f) << 4;
         else
            ch |= ((*psz - '0') & 0x0f) << 4;
         if(*psz == '\0')
            break;
         psz++;
         if(*psz > '9')
            ch |= ((*psz - 'A' + 10) & 0x0f);
         else
            ch |= ((*psz - '0') & 0x0f);
         *pch = ch;
         psz++;
         pch++;
      }
   }

   void memory_base::ToAsc(string & str)
   {
      string strTo;
      To(strTo);
      char ch;
      int iLen = strTo.get_length() - 1;
      for(int i = 0; i < iLen; i+=2)
      {
         if(strTo[i] <= '9')
            ch = (strTo[i] - '0') << 4;
         else
            ch = (strTo[i] - 'A' + 10) << 4;
         if(strTo[i + 1] <= '9')
            ch |= (strTo[i + 1] - '0');
         else
            ch |= (strTo[i + 1] - 'A' + 10);
         str += ch;
      }
   }

   void memory_base::FromAsc(const char * psz)
   {
      string str;
      while(*psz)
      {
         char ch = ((*psz & 0xf0) >> 4);
         if(ch < 10)
            ch += '0';
         else
            ch += 'A' - 10;
         str += ch;
         ch = ((*psz & 0xf));
         if(ch < 10)
            ch += '0';
         else
            ch += 'A' - 10;
         str += ch;
         psz++;
      }
      From(str);
   }


   ::primitive::memory_size memory_base::read(ex1::file & file)
   {
      uint64_t dwEnd = file.get_length();
      uint64_t dwPos = file.get_position();
      memory_size dwRemain = (memory_size)(dwEnd - dwPos);
      allocate((memory_size) dwRemain);
      memory_size dwRead = file.read(get_data(), dwRemain);
      allocate(dwRead);
      return dwRead;
   }


   void memory_base::from_string(const wchar_t * pwsz)
   {
      from_string(gen::international::unicode_to_utf8(pwsz));
   }

   void memory_base::from_string(const char * psz)
   {
      allocate(strlen(psz));
      memcpy(get_data(), psz, this->get_size());
   }

   void memory_base::to_string(string & str)
   {
      LPTSTR lpsz = str.GetBufferSetLength(this->get_size() + 1);
      memcpy(lpsz, get_data(), this->get_size());
      lpsz[this->get_size()] = '\0';
      str.ReleaseBuffer();
   }

   void memory_base::write(ex1::byte_output_stream & ostream)
   {
      ostream.write(get_data(), this->get_size());
   }

   void memory_base::read(ex1::byte_input_stream & istream)
   {
      UINT uiRead;
      UINT uiBufSize = 1024 + 1024;
      UINT uiSize = 0;

      while(true)
      {
         allocate(uiSize + uiBufSize);
         uiRead = istream.read(&get_data()[uiSize], uiBufSize);
         if(uiRead < uiBufSize)
         {
            allocate(uiSize + uiRead);
            break;
         }
         uiSize += uiBufSize;
      }
   }


   void memory_base::append(const void * pdata, memory_size iCount)
   {
      if(iCount <= 0)
         return;
      allocate_add_up(iCount);
      memcpy(&get_data()[this->get_size() - iCount], pdata, (size_t) iCount);
   }


   void memory_base::assign(const void * pdata, memory_position iStart, memory_size iCount)
   {
      if(iCount < 0)
         iCount = 0;
      allocate(iCount);
      memcpy(get_data(), &((LPBYTE)pdata)[iStart], (size_t) iCount);
   }

   void memory_base::append(memory_size iCount, unsigned char uch)
   {
      if(iCount < 0)
         iCount = 0;
      allocate_add_up(iCount);
      memory_position iStart = this->get_size() - iCount;
      memory_position iEnd = iStart + iCount -1;
      for(memory_position i = iStart; i <= iEnd; i++)
      {
         get_data()[i] = uch;
      }
   }

   void memory_base::assign(memory_size iCount, unsigned char uch)
   {
      if(iCount < 0)
         iCount = 0;
      allocate(iCount);
      memory_position iEnd = iCount - 1;
      for(memory_position i = 0; i <= iEnd; i++)
      {
         get_data()[i] = uch;
      }
   }

   unsigned char memory_base::operator [] (int i) const
   {
      return get_data()[i];
   }

   unsigned char & memory_base::operator [] (int i)
   {
      return get_data()[i];
   }

   memory_base::operator const byte *() const
   {
      return get_data();
   }

   memory_base::operator byte *()
   {
      return get_data();
   }

   memory_base::operator const void *() const
   {
      return get_data();
   }

   memory_base::operator void *()
   {
      return get_data();
   }

} // namespace primitive
