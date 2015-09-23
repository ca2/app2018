#include "framework.h"


namespace ios
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

      return Application.dir().userappdata() / ".ca2/cryptkey";

   }


   bool crypto::decrypt(memory & storageDecrypt, const memory & storageEncrypt, const char * pszSalt)
   {
      
      return ::crypto::crypto::decrypt(storageDecrypt, storageEncrypt, pszSalt);
      
      //      memory memOut;
      //
      //      memory memIn;
      //
      //      memIn.append(storageEncrypt.get_data(), storageEncrypt.get_size());
      //
      //      if(!::crypto_decrypt(memOut, memIn, pszSalt))
      //         return false;
      //
      //      storageDecrypt = memOut;
      //
      //      return true;
      
   }
   
   
   bool crypto::encrypt(memory & storageEncrypt, const memory & storageDecrypt, const char * pszSalt)
   {
      
      return ::crypto::crypto::encrypt(storageEncrypt, storageDecrypt, pszSalt);
      
      
      //      memory memOut;
      //
      //      memory memIn;
      //
      //      memIn.append(storageDecrypt.get_data(), storageDecrypt.get_size());
      //
      //      if(!::crypto_encrypt(memOut, memIn, pszSalt))
      //         return false;
      //
      //      storageEncrypt = memOut;
      //
      //      return true;
      
   }


} // namespace ios


