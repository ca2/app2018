/**
 **   \file SSLInitializer.cpp
 **   \date  2007-04-30
 **   \author grymse@alhem.net
**/
/*
Copyright (C) 2007  Anders Hedstrom

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include "framework.h"
#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/rand.h>

#ifdef DEBUG
#define DEB(x) x
#else
#define DEB(x)
#endif


namespace sockets
{


   void rand_seed(const void * buf, int32_t num);
   int32_t rand_bytes(uchar * buf, int32_t num);
   void rand_cleanup();
   void rand_add(const void * buf, int32_t num, double entropy);
   int32_t rand_pseudorand(uchar * buf, int32_t num);
   int32_t rand_status();

   #ifdef LINUX
// ssl_sigpipe_handle ---------------------------------------------------------
void ssl_sigpipe_handle( int x ) {
    /* Ignore broken pipes */
}
   #endif


   RAND_METHOD rand_meth;

   ::plane::system * g_psystem = NULL;

   void rand_seed(const void * buf, int32_t num)
   {
      UNREFERENCED_PARAMETER(buf);
      UNREFERENCED_PARAMETER(num);
   }

   int32_t rand_bytes(uchar * buf, int32_t num)
   {
      g_psystem->math().gen_rand(buf, num);
      return num;
   }

   void rand_cleanup()
   {
   }

   void rand_add(const void * buf, int32_t num, double entropy)
   {
      UNREFERENCED_PARAMETER(buf);
      UNREFERENCED_PARAMETER(num);
      UNREFERENCED_PARAMETER(entropy);
   }

   int32_t rand_pseudorand(uchar * buf, int32_t num)
   {
      g_psystem->math().gen_rand(buf, num);
      return num;
   }

   int32_t rand_status()
   {
      return 1024;
   }


   SSLInitializer::SSLInitializer(sp(base_application) papp) :
      element(papp)
   {



      TRACE("SSLInitializer()\n");

      bio_err = NULL;
      m_rand_size = 1024;

      g_psystem = &Sys(papp->m_psystem);

      /* An error write context */
      bio_err = BIO_new_fp(stderr, BIO_NOCLOSE);

      /* Global system initialization*/
      SSL_library_init();
      SSL_load_error_strings();
      OpenSSL_add_all_algorithms();
      CRYPTO_set_locking_callback(SSL_locking_function);
      CRYPTO_set_id_callback(SSL_id_function);


 /* Ignore broken pipes which would cause our program to terminate
    prematurely */

      rand_meth.add = &rand_add;
      rand_meth.bytes = &rand_bytes;
      rand_meth.cleanup = &rand_cleanup;
      rand_meth.pseudorand = &rand_pseudorand;
      rand_meth.seed = &rand_seed;
      rand_meth.status = &rand_status;

      RAND_set_rand_method(&rand_meth);



   /*   char *randfile =
      char *home = getenv("HOME");
      if (!randfile && !home)
      {
         char *homepath = getenv("HOMEPATH");
         if (homepath)
         {
            Utility::SetEnv("HOME", homepath);
         }
      }*/

      //primitive::memory memstorage;
      //memstorage.allocate(1984 + 1977);
      //memstorage.allocate(1984 + 1977);
      //System.math().gen_rand(memstorage.get_data(), memstorage.get_size());

      /*for(int32_t i = 0; i < memstorage.get_size(); i += 3)
      {
         int32_t iValue = System.math().RandRange(0, 0x00ffffff);
         memstorage.get_data()[i] = iValue & 0xff;
         memstorage.get_data()[i+1] = (iValue >> 8) & 0xff;
         memstorage.get_data()[i+2] = (iValue >> 16) & 0xff;
      }*/

      /*m_rand_file = System.file().time_square();
      //*path = 0;
      //RAND_file_name(path, 512);
      //if (*path)
      //{



      int32_t iWritten = RAND_write_file(m_rand_file);
      m_rand_size = iWritten;
      //}
      //else
      //{
   //TRACE("SSLInitializer: no random file generated\n");
   //   }

      ::file::binary_buffer_sp spfile(allocer());

      spfile->open(m_rand_file, ::file::type_binary | ::file::mode_read);


      //memstorage.FullLoad(spfile);

      /* Load randomness */
      /*if (!m_rand_file.get_length())
      {
   TRACE("SSLInitializer: PRNG not initialized\n");
      }*/
      /*RAND_add(
            memstorage.get_data(),
            memstorage.get_size(),
            memstorage.get_size());*/

      //RAND_seed(memstorage.get_data(), memstorage.get_size());

   }


   SSLInitializer::~SSLInitializer()
   {
      TRACE("~SSLInitializer()\n");
      //DeleteRandFile();
      // %! delete mutexes
   }


   void SSLInitializer::DeleteRandFile()
   {
      if (m_rand_file.get_length())
      {
         System.file().del(m_rand_file);
      }
   }


   void SSLInitializer::SSL_locking_function(int32_t mode, int32_t n, const char * file, int32_t line)
   {
      UNREFERENCED_PARAMETER(file);
      UNREFERENCED_PARAMETER(line);

      static map < int32_t, int32_t, mutex *, mutex *> mmap;
      mutex * pmutex;
      if(!mmap.Lookup(n, pmutex))
      {
         mmap[n] = new mutex(get_thread_app());
      }
      if (mode & CRYPTO_LOCK)
      {
         mmap[n]->lock();
      }
      else
      {
         mmap[n]->unlock();
      }
   }


   unsigned long SSLInitializer::SSL_id_function()
   {
#ifdef WIN32
      return ::GetCurrentThreadId();
#else
      return (unsigned long) (int_ptr) ::pthread_self();
      //return System.get_thread_id();
#endif
   }


} // namespace sockets


#endif // HAVE_OPENSSL
