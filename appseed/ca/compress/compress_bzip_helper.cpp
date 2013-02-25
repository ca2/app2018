#include "framework.h"


//#define ALLOC(size) malloc(size)
//#define TRYFREE(p) {if (p) free(p);}
//#define Z_BUFSIZE 4096
#define ASCII_FLAG   0x01 /* bit 0 set: file probably ascii text */
#define HEAD_CRC     0x02 /* bit 1 set: header CRC present */
#define EXTRA_FIELD  0x04 /* bit 2 set: extra field present */
#define ORIG_NAME    0x08 /* bit 3 set: original file name present */
#define COMMENT      0x10 /* bit 4 set: file comment present */
#define RESERVED     0xE0 /* bits 5..7: reserved */

typedef  uchar GZIP;
typedef  GZIP* LPGZIP;

static const int32_t gz_magic[2] = {0x1f, 0x8b}; /* gzip magic header */

#define BZ_SETERR(err) m_z_err = err
typedef void *(*bzalloc)(void *,int32_t,int32_t);
typedef void (*bzfree)(void *,void *);

bzip::bzip(::ca::file * pfileDest) :
   m_ostream(pfileDest)
{
   construct();
}

bzip::bzip(::ca::writer & writer) :
   m_ostream(&writer)
{
   construct();
}


bzip::bzip(::ca::byte_output_stream & ostream) :
   m_ostream(ostream)
{
   construct();
}

bzip::~bzip()
{
}

bool bzip::write(void * buf, ::primitive::memory_size len)
{
   int32_t n, n2, ret;

   m_z_err = BZ_OK;

   if (len == 0)
   { m_z_err = BZ_OK; return true; };

   m_zstream.avail_in = (uint32_t) len;
   m_zstream.next_in  = (char *) buf;

   while (true) {
      m_zstream.avail_out = BZ_MAX_UNUSED;
      m_zstream.next_out = (char *) m_memory.get_data();
      ret = BZ2_bzCompress ( &(m_zstream), BZ_RUN );
      if (ret != BZ_RUN_OK)
      { BZ_SETERR(ret); return false; };

      if (m_zstream.avail_out < BZ_MAX_UNUSED) {
         n = BZ_MAX_UNUSED - m_zstream.avail_out;
         bool bWriteOk = true;
         try
         {
            m_ostream.write (m_memory.get_data(), n);
            n2 = n;
         }
         catch(...)
         {
            bWriteOk = false;
         }
         if (n != n2 || !bWriteOk)
         { BZ_SETERR(BZ_IO_ERROR); return false; };
      }

      if (m_zstream.avail_in == 0)
      { BZ_SETERR(BZ_OK); return true; };
   }
}

void bzip::construct()
{
   m_CurrentBufferSize = 1024 * 1024 * 8;
   int32_t blockSize100k = 9; // 900k
   int32_t workFactor = 12;
   int32_t verbosity = 0;
   m_memory.allocate(m_CurrentBufferSize);

   m_zstream.bzalloc = (bzalloc)0;
   m_zstream.bzfree = (bzfree)0;
   m_zstream.opaque = (voidpf)0;
   m_zstream.next_in = Z_NULL;
   m_zstream.next_out = Z_NULL;
   m_zstream.avail_in = 0;
   m_zstream.avail_out = 0;
   m_z_err = BZ_OK;
   m_crc = crc32(0L, Z_NULL, 0);
   int32_t err = BZ2_bzCompressInit(&m_zstream, blockSize100k, verbosity, workFactor);
   if (err != Z_OK || m_memory.get_data() == Z_NULL)
   {
      return;
   }
   //GZIP header[10]={0x1f,0x8b,Z_DEFLATED, 0 /*flags*/, 0,0,0,0 /*time*/, 0 /*xflags*/, OS_CODE};
   //m_postream->write(header,10);
   m_zstream.next_out      = (char *) m_memory.get_data();
   m_zstream.avail_out     = (uint32_t) m_memory.get_size();
}





void bzip::finish()
{
   int32_t   n, n2, ret;

   if(m_z_err == BZ_OK)
   {
      while (true)
      {
         m_zstream.avail_out = BZ_MAX_UNUSED;
         m_zstream.next_out = (char *) m_memory.get_data();
         ret = BZ2_bzCompress ( &(m_zstream), BZ_FINISH );
         if (ret != BZ_FINISH_OK && ret != BZ_STREAM_END)
         {
            BZ_SETERR(ret);
            return;
         };

         if (m_zstream.avail_out < BZ_MAX_UNUSED)
         {
            n = BZ_MAX_UNUSED - m_zstream.avail_out;
            bool bWriteOk = true;
            try
            {
               m_ostream.write((char *) m_memory.get_data(), n);
               n2 = n;
            }
            catch(...)
            {
               bWriteOk = false;
            }
            if (n != n2 || !bWriteOk)
            {
               BZ_SETERR(BZ_IO_ERROR);
               return;
            }
         }

         if (ret == BZ_STREAM_END) break;
      }
   }

//   m_postream->flush();

   BZ_SETERR(BZ_OK);
   BZ2_bzCompressEnd ( &(m_zstream) );

}

