#include "framework.h"
#include "aura/aura/crypto/crypto_openssl.h"
#include <stdio.h>
#include <openssl/md5.h>


/////////////////////////////////////////////////////
//
//    md5 implementation
//    intentional duplication because spa
//    needs md5
//
string file_md5_dup(const char * psz)
{
   unsigned char * bufMd5 = NULL;
   FILE * f = fopen(psz,"rb");
   if(!f)
      return "";
   int iBufSize = 1024 * 1024;
   if(bufMd5 == NULL)
   {
      bufMd5 = new unsigned char[iBufSize];
   }
   unsigned char * buf = bufMd5;
   MD5_CTX ctx;
   MD5_Init(&ctx);
   UINT_PTR iRead;
   while((iRead = fread(buf,1,iBufSize,f)) > 0)
   {
      MD5_Update(&ctx, buf,iRead);
   }
   fclose(f);
   return ::str::from(ctx);
}

string str_md5_dup(const char * psz)
{
   MD5_CTX ctx;
   MD5_Init(&ctx);
   MD5_Update(&ctx,   (void *) psz,strlen(psz));
   return str::from(ctx);
}

