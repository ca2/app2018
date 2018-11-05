#pragma once


namespace macos
{


   class CLASS_DECL_AXIS crypto :
      virtual public ::crypto::crypto
   {
   public:


      crypto(sp(::aura::application) papp);
      virtual ~crypto();


      virtual bool encrypt(memory & storageEncrypt, const memory & storageDecrypt, const char * pszSalt);
      virtual bool decrypt(memory & storageDecrypt, const memory & storageEncrypt, const char * pszSalt);


      virtual ::file::path get_crypt_key_file_path();



   };

   
} // namespace macos





