#include "StdAfx.h"

//#define ALLOC(size) malloc(size)
//#define TRYFREE(p) {if (p) free(p);}
//#define Z_BUFSIZE 4096
#define ASCII_FLAG   0x01 /* bit 0 set: file probably ascii text */
#define HEAD_CRC     0x02 /* bit 1 set: header CRC present */
#define EXTRA_FIELD  0x04 /* bit 2 set: extra field present */
#define ORIG_NAME    0x08 /* bit 3 set: original file name present */
#define COMMENT      0x10 /* bit 4 set: file comment present */
#define RESERVED     0xE0 /* bits 5..7: reserved */

typedef  unsigned char GZIP;
typedef  GZIP* LPGZIP;

static const int gz_magic[2] = {0x1f, 0x8b}; /* gzip magic header */


gzip::gzip(ex1::file * pfileDest)
{
   m_postream = pfileDest;
   construct();
}
gzip::gzip(ex1::output_stream & ostreamDest)
{
   m_postream = &ostreamDest;
   construct();
}
gzip::~gzip()
{
}

   bool gzip::write(void * buf, int iSize)
   {
      m_crc = crc32(m_crc, (const Bytef *) buf, iSize);
      m_zstream.next_in       = (Bytef*)buf;
      m_zstream.avail_in      = iSize;
      while (m_zstream.avail_in != 0)
      {
         if(m_zstream.avail_out == 0)
         {
            m_zstream.next_out = m_memory.GetAllocation();
            m_postream->write(m_memory.GetAllocation(), m_memory.GetStorageSize());
            m_zstream.avail_out = m_memory.GetStorageSize();
        }
        m_z_err = deflate(&(m_zstream), Z_NO_FLUSH);
        if(m_z_err != Z_OK) 
           break;
      }
      return true;
   }

   void gzip::construct()
   {
      int iLevel = Z_BEST_COMPRESSION;
      int iStrategy = Z_DEFAULT_STRATEGY;
      m_memory.allocate(1024 * 1024 * 8);

      m_zstream.zalloc = (alloc_func)0;
      m_zstream.zfree = (free_func)0;
      m_zstream.opaque = (voidpf)0;
      m_zstream.next_in = Z_NULL;
      m_zstream.next_out = Z_NULL;
      m_zstream.avail_in = 0;
      m_zstream.avail_out = 0;
      m_z_err = Z_OK;
      m_crc = crc32(0L, Z_NULL, 0);
      int err = deflateInit2(&(m_zstream), iLevel,Z_DEFLATED, -MAX_WBITS, DEF_MEM_LEVEL, iStrategy);
      if (err != Z_OK || m_memory.GetAllocation() == Z_NULL)
      {
         destroy();
         return;
      }
      GZIP header[10]={0x1f,0x8b,Z_DEFLATED, 0 /*flags*/, 0,0,0,0 /*time*/, 0 /*xflags*/, OS_CODE};
      m_postream->write(header,10);
      m_zstream.next_out      = m_memory.GetAllocation();
      m_zstream.avail_out     = m_memory.GetStorageSize();
   }





   void gzip::finish()
   {
      if(_finish() != Z_OK) 
      { 
         destroy(); 
         return;
      }
      putLong(m_crc);
      putLong (m_zstream.total_in);  
      destroy();
   }

   int gzip::_finish()
   {
      int done = 0;
      int len = 0;
      m_zstream.avail_in = 0; /* should be zero already anyway */
      for (;;)
      {
         len = m_memory.GetStorageSize() - m_zstream.avail_out;

         if (len != 0)
         {
            m_postream->write(m_memory.GetAllocation(), len);
            m_zstream.next_out = m_memory.GetAllocation();
            m_zstream.avail_out = m_memory.GetStorageSize();
         }
         if(done)
            break;
         m_z_err = deflate(&(m_zstream), Z_FINISH);

         /* Ignore the second of two consecutive flushes: */
         if(len == 0 && m_z_err == Z_BUF_ERROR)
            m_z_err = Z_OK;

         /* deflate has finished flushing only when it hasn't used up
         * all the available space in the output buffer:
         */
         done = (m_zstream.avail_out != 0 || m_z_err == Z_STREAM_END);

         if (m_z_err != Z_OK && m_z_err != Z_STREAM_END)
            break;
      }
      return  m_z_err == Z_STREAM_END ? Z_OK : m_z_err;
   }
   int gzip::destroy()
   {
      int err = Z_OK;
      if (m_zstream.state != NULL) {
         err = deflateEnd(&(m_zstream));
      }
      if (m_z_err < 0) err = m_z_err;
      return err;
   }

   void gzip::putLong (uLong x)
   {
      for(int n = 0; n < 4; n++) {
         unsigned char c=(unsigned char)(x & 0xff);
         m_postream->write(&c,1);
         x >>= 8;
      }
   }





   
