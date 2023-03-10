#include "framework.h"
//#include <shlobj.h>
//#include <Security.h>
//#include <shlobj.h>


//#include "windows_dir.h"
//#include "windows_factory_exchange.h"
//#include "windows_port_forward.h"
//#include "windows_dir.h"
//#include "windows_file_system.h"
//#include "windows_file_set.h"
#include "windows_crypto.h"
//#include "windows_ip_enum.h"

int32_t crypto_encrypt(memory & storageEncrypt, const memory & storageDecrypt, memory & key);
int32_t crypto_decrypt(memory & storageDecrypt, const memory & storageEncrypt, memory & key);

namespace windows
{


   crypto::crypto(::aura::application * papp) :
      ::object(papp),
      ::crypto::crypto(papp)
   {
   }



   crypto::~crypto()
   {

   }


   ::file::path crypto::get_crypt_key_file_path()
   {

      return System.dir().appdata() / ".ca2/databin.bin";

   }


   bool crypto::decrypt(memory & storageDecrypt, const memory & storageEncrypt, const char * pszSalt)
   {

      return ::crypto::crypto::decrypt(storageDecrypt, storageEncrypt, pszSalt);

#if 0

      DATA_BLOB DataIn;
      DATA_BLOB DataOut;

      if (pszSalt == NULL)
         pszSalt = "";

      DATA_BLOB DataSalt;
      memory memorySalt;
      memorySalt.from_string(pszSalt);
      DataSalt.pbData = memorySalt.get_data();
      DataSalt.cbData = (DWORD)memorySalt.get_size();

      //--------------------------------------------------------------------
      // Initialize the DataIn structure.

      DataIn.pbData = storageEncrypt.get_data();
      DataIn.cbData = (DWORD)storageEncrypt.get_size();

      unichar * lpwsz = NULL;

      //--------------------------------------------------------------------
      //  begin protect phase. Note that the encryption key is created
      //  by the function and is not passed.

      if (CryptUnprotectData(
            &DataIn,
            NULL, // A description string
            // to be included with the
            // encrypted data.
            &DataSalt,                               // Optional entropy not used.
            NULL,                               // Reserved.
            NULL,                               // Pass NULL for the
            // prompt structure.
            0,
            &DataOut))
      {
         TRACE("crypto::decrypt The encryption phase worked. \n");
         storageDecrypt.allocate(DataOut.cbData);
         memcpy(storageDecrypt.get_data(), DataOut.pbData, DataOut.cbData);
         LocalFree(lpwsz);
         LocalFree(DataOut.pbData);
         return true;
      }
      else
      {
         //         TRACELASTERROR();
         TRACE("crypto::decrypt Decryption error! (1)");
         return false;
      }
#else
      memory memOut;

      memory memIn;

      memIn.append(storageEncrypt.get_data(), storageEncrypt.get_size());

      memory memSalt;

      memSalt.append(pszSalt, strlen(pszSalt));

      if (!::crypto_decrypt(memOut, memIn, memSalt))
         return false;


      storageDecrypt = memOut;

      return true;

#endif
   }

   bool crypto::encrypt(memory & storageEncrypt, const memory & storageDecrypt, const char * pszSalt)
   {

      return ::crypto::crypto::encrypt(storageEncrypt, storageDecrypt, pszSalt);
#if 0
      DATA_BLOB DataIn;
      DATA_BLOB DataOut;

      if (pszSalt == NULL)
         pszSalt = "";

      DATA_BLOB DataSalt;
      memory memorySalt;
      memorySalt.from_string(pszSalt);
      DataSalt.pbData = memorySalt.get_data();
      DataSalt.cbData = (DWORD)memorySalt.get_size();


      //--------------------------------------------------------------------
      // Initialize the DataIn structure.

      DataIn.pbData = (BYTE *)storageDecrypt.get_data();
      DataIn.cbData = (DWORD)storageDecrypt.get_size();

      //      unichar * lpwsz = NULL;

      //--------------------------------------------------------------------
      //  begin protect phase. Note that the encryption key is created
      //  by the function and is not passed.

      if (CryptProtectData(
            &DataIn,
            NULL, // A description string
            // to be included with the
            // encrypted data.
            &DataSalt,                               // Optional entropy not used.
            NULL,                               // Reserved.
            NULL,                               // Pass NULL for the
            // prompt structure.
            0,
            &DataOut))
      {
         TRACE("crypto::encrypt The encryption phase worked. \n");
         storageEncrypt.allocate(DataOut.cbData);
         memcpy(storageEncrypt.get_data(), DataOut.pbData, DataOut.cbData);
         LocalFree(DataOut.pbData);
         return true;
      }
      else
      {
         TRACE("crypto::encrypt Encryption error! (1)");
         return false;
      }
#else

      memory memOut;

      memory memIn;

      memIn.append(storageDecrypt.get_data(), storageDecrypt.get_size());

      memory memSalt;

      memSalt.append(pszSalt, strlen(pszSalt));

      if (!::crypto_encrypt(memOut, memIn, memSalt))
         return false;


      storageEncrypt = memOut;

      return true;
#endif

   }


} // namespace windows



