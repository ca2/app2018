#include "framework.h"


namespace crypto
{


   namespace md5 
   {


      context::context(sp(base_application) papp) :
         element(papp),
         m_memoryDigest(papp)
      {

         m_bEnd = false;

         

#ifdef METROWIN

         m_posdata = new os_data;

         ::Windows::Security::Cryptography::Core::HashAlgorithmProvider ^ provider = 
            ::Windows::Security::Cryptography::Core::HashAlgorithmProvider::OpenAlgorithm(
               ::Windows::Security::Cryptography::Core::HashAlgorithmNames::Md5);

         m_posdata->m_hash = provider->CreateHash();

#else

         MD5_Init(&m_ctx);

#endif

      }


      context::~context()
      {

#ifdef METROWIN

         if (m_posdata != NULL)
         {

            delete m_posdata;
            m_posdata = NULL;

         }

#endif

      }


      void context::update(const void * data, size_t size)
      {

         if(m_bEnd)
            throw "invalid state exception";

         if (data == NULL || size == 0)
            return;

#ifdef METROWIN


         m_posdata->m_hash->Append(::Windows::Security::Cryptography::CryptographicBuffer::CreateFromByteArray(ref new Platform::Array < uchar, 1U > ((uchar *) data, size)));

#else

         size_t pos = 0;

         size_t read;

         while(pos < size)
         {

            read = min(size - pos, 0xffffffffu);

            MD5_Update(&m_ctx, &((byte *) data)[pos], read);

            pos += read;

         }

#endif

      }

      void context::get(primitive::memory_base & mem) // 16 bytes - 32 hex characters
      {

         defer_finalize();

         mem = m_memoryDigest;

      }


      string context::to_hex() // 16 bytes - 32 hex characters
      {

         defer_finalize();

         return m_memoryDigest.to_hex();

      }

      void context::defer_finalize()
      {

         if(!m_bEnd)
         {

#ifdef METROWIN

            m_memoryDigest.set_os_buffer(m_posdata->m_hash->GetValueAndReset());

#else
         
            m_memoryDigest.allocate(16);

            MD5_Final(m_memoryDigest.get_data(), &m_ctx);

#endif

            m_bEnd = true;

         }

      }

      void context::reset()
      {

         if(!m_bEnd)
         {

#ifdef METROWIN

            m_posdata->m_hash->GetValueAndReset();

#endif

         }

#ifndef METROWIN

         MD5_Init(&m_ctx);

#endif

         m_bEnd = false;

      }

      

   } // namespace md5


} // namespace crypto


