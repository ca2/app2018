#include "framework.h"
#include "linux.h"


namespace linux
{


   crypto::crypto(sp(::aura::application) papp) :
      ::object(papp),
      ::crypto::crypto(papp)
   {
   }


   crypto::~crypto()
   {
   }


   ::file::path crypto::get_crypt_key_file_path()
   {

      return ::file::path(getenv("home")) / ".ca2/cryptkey";

   }


   bool crypto::decrypt(primitive::memory & storageDecrypt, const primitive::memory & storageEncrypt, const char * pszSalt)
   {

      ::primitive::memory memOut;

      ::primitive::memory memIn;

      memIn.append(storageEncrypt.get_data(), storageEncrypt.get_size());

      ::primitive::memory memSalt;

      memSalt.append(pszSalt, strlen(pszSalt));

      if(!::crypto_decrypt(memOut, memIn, memSalt))
         return false;


      storageDecrypt = memOut;

      return true;

   }

   bool crypto::encrypt(primitive::memory & storageEncrypt, const primitive::memory & storageDecrypt, const char * pszSalt)
   {

      ::primitive::memory memOut;

      ::primitive::memory memIn;

      memIn.append(storageDecrypt.get_data(), storageDecrypt.get_size());

      ::primitive::memory memSalt;

      memSalt.append(pszSalt, strlen(pszSalt));

      if(!::crypto_encrypt(memOut, memIn, memSalt))
         return false;


      storageEncrypt = memOut;

      return true;

   }


} // namespace linux


