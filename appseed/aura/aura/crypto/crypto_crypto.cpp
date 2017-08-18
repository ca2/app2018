#include "framework.h"

#if defined(OPENSSL_CRYPTO) || defined(METROWIN)

#include <openssl/rsa.h>
#include <openssl/md5.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/hmac.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <openssl/whrlpool.h>


#endif

#if defined(HAVE_OPENSSL) || defined(METROWIN)

#include <openssl/ssl.h>

#endif

#define CA4_CRYPT_V5_FINAL_HASH_BYTES (WHIRLPOOL_DIGEST_LENGTH * 16)
#define CA4_CRYPT_V5_SALT_BYTES (CA4_CRYPT_V5_FINAL_HASH_BYTES - WHIRLPOOL_DIGEST_LENGTH)

string chunk_split(const string & body,int32_t chunklen = 76,const string & end = "\r\n");
namespace crypto
{


   crypto::crypto(::aura::application * papp) :
      ::object(papp)
   {
   }

   crypto::~crypto()
   {
   }


   bool crypto::decrypt(memory & storageDecrypt, const memory & storageEncrypt, const char * pszSalt)
   {

      // default implementation - OS may implement its own HOME/UserDir encryption

      string str = defer_get_cryptkey();

      if (str.is_empty())
         return false;

      str += pszSalt;

      memory key(get_app());

      key.from_string(str);

      return decrypt(storageDecrypt, storageEncrypt, key);

   }

   bool crypto::encrypt(memory & storageEncrypt, const memory & storageDecrypt, const char * pszSalt)
   {

      // default implementation - OS may implement its own HOME/UserDir encryption

      string str = defer_get_cryptkey();

      if (str.is_empty())
         return false;

      str += pszSalt;

      memory key(get_app());

      key.from_string(str);

      return encrypt(storageEncrypt, storageDecrypt, key);

   }



   int32_t crypto::key(memory & storage)
   {
      storage.allocate(16);
      for (memory_position_t i = 0; i < storage.get_size(); i++)
      {
         storage.get_data()[i] = System.math().RandRange(0, 255) & 0xff;
      }
      return (int32_t)storage.get_size();
   }

   /**
   AES encryption/decryption demo program using OpenSSL EVP apis
   gcc -Wall openssl_aes.ca -lcrypto

   this is public domain code.

   Saju Pillai (saju.pillai@gmail.com)
   **/
   //http://stackoverflow.com/questions/10366950/openssl-using-evp-vs-algorithm-api-for-symmetric-crypto

   bool crypto::encrypt(memory & storageEncrypt, const memory & storageDecrypt, const memory & memKeyData)
   {

      memory memSha1(get_app());

      nessie(memSha1,memKeyData);

      memory iv(get_app());

      iv.allocate(32);

      iv.set(0);

#ifdef OPENSSL_CRYPTO


      int32_t plainlen = (int32_t)storageDecrypt.get_size();

      int32_t cipherlen,tmplen;

      EVP_CIPHER_CTX * pctx = EVP_CIPHER_CTX_new();

      EVP_EncryptInit(pctx,EVP_aes_256_ecb(),memSha1.get_data(),iv.get_data());

      cipherlen = (int32_t)(storageDecrypt.get_size() + EVP_CIPHER_CTX_block_size(pctx));

      storageEncrypt.allocate(cipherlen);

      storageEncrypt.set(0);

      if(!EVP_EncryptUpdate(pctx,storageEncrypt.get_data(),&cipherlen,storageDecrypt.get_data(),plainlen))
      {

         storageEncrypt.set(0);

         storageEncrypt.allocate(0);

#if OPENSSL_API_COMPAT < 0x10100000L
         
         EVP_CIPHER_CTX_cleanup(pctx);
         
#else
         
         EVP_CIPHER_CTX_reset(pctx);
         
#endif
         
         return false;

      }

      if(!EVP_EncryptFinal(pctx,storageEncrypt.get_data() + cipherlen,&tmplen))
      {

         storageEncrypt.set(0);

         storageEncrypt.allocate(0);

#if OPENSSL_API_COMPAT < 0x10100000L
         
         EVP_CIPHER_CTX_cleanup(pctx);
         
#else
         
         EVP_CIPHER_CTX_reset(pctx);
         
#endif
         
         return false;

      }

      cipherlen += tmplen;

      storageEncrypt.allocate(cipherlen);

#if OPENSSL_API_COMPAT < 0x10100000L
      
      EVP_CIPHER_CTX_cleanup(pctx);
      
#else
      
      EVP_CIPHER_CTX_reset(pctx);
      
#endif
      
      return true;


#elif defined(METROWIN)

      ::Windows::Security::Cryptography::Core::SymmetricKeyAlgorithmProvider ^ cipher =
         ::Windows::Security::Cryptography::Core::SymmetricKeyAlgorithmProvider::OpenAlgorithm(::Windows::Security::Cryptography::Core::SymmetricAlgorithmNames::AesEcb);

      ::Windows::Security::Cryptography::Core::CryptographicKey ^ cipherkey = cipher->CreateSymmetricKey(memSha1.get_os_crypt_buffer());

      storageEncrypt.set_os_crypt_buffer(::Windows::Security::Cryptography::Core::CryptographicEngine::Encrypt(cipherkey, storageDecrypt.get_os_crypt_buffer(), iv.get_os_crypt_buffer()));

      return true;

#elif defined(MACOS)

      CFMutableDictionaryRef parameters = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

      CFDictionaryAddValue(parameters, kSecAttrKeyType, kSecAttrKeyTypeAES);

      CFDataRef keyData = memSha1.get_os_cf_data();

      CFErrorRef error = NULL;

      SecKeyRef key = SecKeyCreateFromData(parameters, keyData, &error);

      if(error != NULL)
      {

         CFRelease(error);

         CFRelease(keyData);

         CFRelease(parameters);

         return false;

      }

      SecTransformRef transform = SecEncryptTransformCreate(key, &error);

      if(error != NULL)
      {

         CFRelease(error);

         CFRelease(key);

         CFRelease(keyData);

         CFRelease(parameters);

         return false;

      }

      SecTransformSetAttribute(transform, kSecPaddingKey, kSecPaddingPKCS1Key, &error);

      if(error != NULL)
      {

         CFRelease(transform);

         CFRelease(keyData);

         CFRelease(parameters);

         CFRelease(key);

         CFRelease(error);

         return false;

      }

      CFDataRef dataIv = iv.get_os_cf_data();

      SecTransformSetAttribute(transform, kSecIVKey, dataIv, &error);

      if(error != NULL)
      {

         CFRelease(dataIv);

         CFRelease(transform);

         CFRelease(parameters);

         CFRelease(keyData);

         CFRelease(key);

         CFRelease(error);

         return false;

      }

      CFDataRef dataIn = storageDecrypt.get_os_cf_data();

      SecTransformSetAttribute(transform, kSecTransformInputAttributeName, dataIn, &error);

      if(error != NULL)
      {

         CFRelease(dataIn);

         CFRelease(dataIv);

         CFRelease(transform);

         CFRelease(parameters);

         CFRelease(keyData);

         CFRelease(key);

         CFRelease(error);

         return false;

      }

      /* Encrypt the data. */

      CFDataRef data = (CFDataRef) SecTransformExecute(transform, &error);

      if(error != NULL)
      {

         CFRelease(dataIn);

         CFRelease(dataIv);

         CFRelease(transform);

         CFRelease(parameters);

         CFRelease(keyData);

         CFRelease(key);

         CFRelease(error);

         return false;

      }

      memory memory;

      storageEncrypt.set_os_cf_data(data);

      CFRelease(data);

      CFRelease(dataIv);

      CFRelease(dataIn);

      CFRelease(transform);

      CFRelease(keyData);

      CFRelease(parameters);

      CFRelease(key);

      return true;


#endif

   }


   bool crypto::decrypt(memory & storageDecrypt, const memory & storageEncrypt, const memory & memKeyData)
   {

      memory memSha1;

      nessie(memSha1, memKeyData);

      memory iv;

      iv.allocate(32);

      iv.set(0);

#if defined(METROWIN) && !defined(OPENSSL_CRYPTO)

      ::Windows::Security::Cryptography::Core::SymmetricKeyAlgorithmProvider ^ cipher =
         ::Windows::Security::Cryptography::Core::SymmetricKeyAlgorithmProvider::OpenAlgorithm(::Windows::Security::Cryptography::Core::SymmetricAlgorithmNames::AesEcb);

      ::Windows::Security::Cryptography::Core::CryptographicKey ^ cipherkey = cipher->CreateSymmetricKey(memSha1.get_os_crypt_buffer());

      storageDecrypt.set_os_crypt_buffer(::Windows::Security::Cryptography::Core::CryptographicEngine::Decrypt(cipherkey, storageDecrypt.get_os_crypt_buffer(), iv.get_os_crypt_buffer()));

      return true;

//#elif defined(MACOS)
//
//      CFMutableDictionaryRef parameters = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
//
//      CFDictionaryAddValue(parameters, kSecAttrKeyType, kSecAttrKeyTypeAES);
//
//      CFDataRef keyData = memSha1.get_os_cf_data();
//
//      CFErrorRef error = NULL;
//
//      SecKeyRef key = SecKeyCreateFromData(parameters, keyData, &error);
//
//      if(error != NULL)
//      {
//
//         CFRelease(error);
//
//         CFRelease(keyData);
//
//         CFRelease(parameters);
//
//         return false;
//
//      }
//
//      SecTransformRef transform = SecDecryptTransformCreate(key, &error);
//
//      if(error != NULL)
//      {
//
//         CFRelease(error);
//
//         CFRelease(key);
//
//         CFRelease(keyData);
//
//         CFRelease(parameters);
//
//         return false;
//
//      }
//
//      SecTransformSetAttribute(transform, kSecPaddingKey, kSecPaddingPKCS1Key, &error);
//
//      if(error != NULL)
//      {
//
//         CFRelease(transform);
//
//         CFRelease(keyData);
//
//         CFRelease(parameters);
//
//         CFRelease(key);
//
//         CFRelease(error);
//
//         return false;
//
//      }
//
//      CFDataRef dataIv = iv.get_os_cf_data();
//
//      SecTransformSetAttribute(transform, kSecIVKey, dataIv, &error);
//
//      if(error != NULL)
//      {
//
//         CFRelease(dataIv);
//
//         CFRelease(transform);
//
//         CFRelease(parameters);
//
//         CFRelease(keyData);
//
//         CFRelease(key);
//
//         CFRelease(error);
//
//         return false;
//
//      }
//
//      CFDataRef dataIn = storageEncrypt.get_os_cf_data();
//
//      SecTransformSetAttribute(transform, kSecTransformInputAttributeName, dataIn, &error);
//
//      if(error != NULL)
//      {
//
//         CFRelease(dataIn);
//
//         CFRelease(dataIv);
//
//         CFRelease(transform);
//
//         CFRelease(parameters);
//
//         CFRelease(keyData);
//
//         CFRelease(key);
//
//         CFRelease(error);
//
//         return false;
//
//      }
//
//      /* Encrypt the data. */
//
//      CFDataRef data = (CFDataRef) SecTransformExecute(transform, &error);
//
//      if(error != NULL)
//      {
//
//         CFRelease(dataIn);
//
//         CFRelease(dataIv);
//
//         CFRelease(transform);
//
//         CFRelease(parameters);
//
//         CFRelease(keyData);
//
//         CFRelease(key);
//
//         CFRelease(error);
//
//         return false;
//
//      }
//
//      memory memory;
//
//      storageDecrypt.set_os_cf_data(data);
//
//      CFRelease(data);
//
//      CFRelease(dataIv);
//
//      CFRelease(dataIn);
//
//      CFRelease(transform);
//
//      CFRelease(keyData);
//
//      CFRelease(parameters);
//
//      CFRelease(key);
//
//      return true;

#else

      int32_t cipherlen = (int32_t)storageEncrypt.get_size();

      int32_t plainlen, tmplen;

      EVP_CIPHER_CTX * pctx = EVP_CIPHER_CTX_new();



      int iKeyLen = EVP_CIPHER_key_length(EVP_aes_256_ecb());
      memory_size_t iShaLen = memSha1.get_size();

      EVP_DecryptInit(pctx, EVP_aes_256_ecb(), memSha1.get_data(), iv.get_data());

      plainlen = (int32_t)storageEncrypt.get_size() + EVP_CIPHER_CTX_block_size(pctx);

      storageDecrypt.allocate(plainlen);

      if (!EVP_DecryptUpdate(pctx, storageDecrypt.get_data(), &plainlen, storageEncrypt.get_data(), cipherlen))
      {

         storageDecrypt.set(0);

         storageDecrypt.allocate(0);

         EVP_CIPHER_CTX_free(pctx);

         return false;

      }

      if (!EVP_DecryptFinal(pctx, storageDecrypt.get_data() + plainlen, &tmplen))
      {

         storageDecrypt.set(0);

         storageDecrypt.allocate(0);

         EVP_CIPHER_CTX_free(pctx);

         return false;

      }

      plainlen += tmplen;

      storageDecrypt.allocate(plainlen);

      EVP_CIPHER_CTX_free(pctx);

      return true;

#endif

   }

   string crypto::strkey()
   {
      memory storage;
      key(storage);
      return System.base64().encode(storage);
   }

   int32_t crypto::encrypt(string & strEncrypt, const char * pszDecrypt, const char * pszKey)
   {
      memory storageDecrypt;
      memory storageEncrypt;
      memory storageKey;
      if (pszDecrypt == NULL || strlen(pszDecrypt) == 0)
      {
         strEncrypt = "";
         return 0;
      }
      storageDecrypt.from_string(pszDecrypt);
      System.base64().decode(storageKey, pszKey);
      int32_t cipherlen = encrypt(storageEncrypt, storageDecrypt, storageKey);
      strEncrypt = System.base64().encode(storageEncrypt);
      return cipherlen;
   }

   int32_t crypto::decrypt(string & strDecrypt, const char * pszEncrypt, const char * pszKey)
   {
      memory storageEncrypt;
      memory storageDecrypt;
      memory storageKey;
      System.base64().decode(storageEncrypt, pszEncrypt);
      System.base64().decode(storageKey, pszKey);
      int32_t plainlen = decrypt(storageDecrypt, storageEncrypt, storageKey);
      storageDecrypt.to_string(strDecrypt);
      return plainlen;
   }


   string crypto::md5(const char * psz)
   {

      memory mem;

      mem.assign(psz,strlen(psz));

      return md5(mem);

   }


   string crypto::sha1(const char * psz)
   {

      memory mem;

      mem.assign(psz,strlen(psz));

      return sha1(mem);

   }


   string crypto::nessie(const char * psz)
   {

      memory mem;

      mem.assign(psz,strlen(psz));

      return nessie(mem);

   }


   string crypto::md5(const memory & mem)
   {

      memory memMd5;

      md5(memMd5, mem);

      return memMd5.to_hex();


   }



   string crypto::sha1(const memory & mem)
   {

      memory memSha1;

      sha1(memSha1, mem);

      return memSha1.to_hex();

   }

   string crypto::nessie(const memory & mem)
   {

      memory memNessie;

      nessie(memNessie,mem);

      return memNessie.to_hex();

   }


   void crypto::md5(memory & memMd5,const memory & mem)
   {

//#ifdef METROWIN
//
//      throw interface_only_exception(get_app());
//
//#else

      memMd5.allocate(MD5_DIGEST_LENGTH);

      MD5(mem.get_data(),mem.get_size(),memMd5.get_data());

//#endif

   }


   void crypto::sha1(memory & memSha1, const memory & mem)
   {

//#ifdef METROWIN
//
//      throw interface_only_exception(get_app());
//
//#else

      memSha1.allocate(SHA_DIGEST_LENGTH);

      SHA1(mem.get_data(), mem.get_size(), memSha1.get_data());

//#endif

   }

   void crypto::nessie(memory & memNessie,const memory & mem)
   {

      memNessie.allocate(WHIRLPOOL_DIGEST_LENGTH);

      WHIRLPOOL(mem.get_data(),mem.get_size(),memNessie.get_data());

   }




   bool crypto::file_set(var varFile, const char * pszData, const char * pszSalt, ::aura::application * papp)
   {
      memory memoryEncrypt;
      encrypt(memoryEncrypt, pszData, pszSalt);
      Sess(papp).file().put_contents(varFile, memoryEncrypt);
      return true;
   }

   bool crypto::file_get(var varFile, string & str, const char * pszSalt, ::aura::application * papp)
   {
      memory memoryEncrypt;
      Sess(papp).file().as_memory(varFile, memoryEncrypt);
      decrypt(str, memoryEncrypt, pszSalt);
      return true;
   }

   bool crypto::encrypt(memory & storageEncrypt, const char * pszDecrypt, const char * pszSalt)
   {
      memory memoryDecrypt;
      memoryDecrypt.from_asc(pszDecrypt);
      return encrypt(storageEncrypt, memoryDecrypt, pszSalt);
   }

   bool crypto::decrypt(string & strDecrypt, const memory & storageEncrypt, const char * pszSalt)
   {
      memory memoryDecrypt;
      if (!decrypt(memoryDecrypt, storageEncrypt, pszSalt))
         return false;
      memoryDecrypt.to_asc(strDecrypt);
      return true;
   }


   // get a new salt - 8 hexadecimal characters long
   // current PHP installations should not exceed 8 characters
   // on dechex( mt_rand() )
   // but we future proof it anyway with substr()
   string crypto::v5_get_password_salt()
   {
      string strSalt;
      string strFormat;
      for(int32_t i = 0; i < CA4_CRYPT_V5_FINAL_HASH_BYTES - WHIRLPOOL_DIGEST_LENGTH; i += 2)
      {
         int64_t iDigit = System.math().RandRange(0, 255);
         strFormat.Format("%02x", iDigit);
         strSalt += strFormat;
      }
      return strSalt;
   }

   // calculate the hash from a salt and a password
   // slow hash is more secure for personal attack possibility (strong fast hashs are only good for single transactional operations and not for a possibly lifetime password)
   string crypto::v5_get_password_hash(const char * pszSalt, const char * pszPassword, int32_t iOrder)
   {
      string strHash(pszPassword);
      string strSalt(pszSalt);
      strSalt = strSalt.Left(CA4_CRYPT_V5_SALT_BYTES);
      for(int32_t i = iOrder; i < CA4_CRYPT_V5_FINAL_HASH_BYTES - WHIRLPOOL_DIGEST_LENGTH; i++)
      {
         string strStepSalt = strSalt.Mid(i) + strSalt.Left(i);
         strHash = nessie(strStepSalt + strHash).lowered();
      }
      return strSalt + strHash;
   }

   string crypto::v5_get_passhash(const char * pszSalt, const char * pszPassword, int32_t iMaxOrder)
   {
      string strHash(pszPassword);
      string strSalt(pszSalt);
      strSalt = strSalt.Left(CA4_CRYPT_V5_SALT_BYTES);
      for (int32_t i = 0; i < iMaxOrder; i++)
      {
         string strStepSalt = strSalt.Mid(i) + strSalt.Left(i);
         strHash = nessie(strStepSalt + strHash).lowered();
      }
      return strSalt + strHash;
   }

   bool crypto::v5_compare_password(const char * pszPassword, const char * pszHash, int32_t iOrder)
   {
      string strHash(pszHash);
      string strSalt = strHash.Left(CA4_CRYPT_V5_SALT_BYTES);
      return strHash == v5_get_password_hash(strSalt, pszPassword, iOrder);
   }

   bool crypto::v5_validate_plain_password(const char * pszPassword)
   {
      string str(pszPassword);
      if (str.get_length() < 6)
         return false;
      return ::str::has_all_v1(pszPassword);
   }

   string crypto::v5_get_password_hash(const char * pszPassword, int32_t iOrder)
   {
      return v5_get_password_hash(v5_get_password_salt(), pszPassword, iOrder);
   }



   void crypto::hmac(void * result, const memory & memMessage, const memory & memKey)
   {

//#ifndef METROWIN

      unsigned int md_len = 0;

      HMAC(EVP_sha1(), memKey.get_data(), memKey.get_size(), memMessage.get_data(), memMessage.get_size(), (unsigned char *) result, &md_len);

//#endif

   }




   void crypto::hmac(void * result,const string & strMessage,const string & strKey)
   {

//#ifndef METROWIN

      unsigned int md_len = 0;

      HMAC(EVP_sha1(),strKey,strKey.length(),(const unsigned char *)(const char *)strMessage,strMessage.length(),(unsigned char *)result,&md_len);

//#endif

   }


   ::file::path crypto::get_crypt_key_file_path()
   {

      return Application.dir().userappdata() / "user/databin.bin";

   }


   string crypto::defer_get_cryptkey()
   {

      string strPath = get_crypt_key_file_path();

      string str = Application.file().as_string(strPath);

      if (str.has_char())
         return str;

      for (int32_t i = 0; i < 256; i++)
      {

         char ch = System.math().rnd() % (10 + 26 + 26);

         if (ch < 10)
            ch += '0';
         else if (ch < (10 + 26))
            ch += 'a' - 10;
         else
            ch += 'A' - (10 + 26);

         str += ch;

      }

      Application.file().put_contents(strPath, str);

      return str;


   }


#ifdef MACOS_DEPRECATED


   SecKeyRef crypto::get_new_rsa_key()
   {

      CFMutableDictionaryRef parameters = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

      CFDictionaryAddValue(parameters, kSecAttrKeyType, kSecAttrKeyTypeRSA);

      CFDictionaryAddValue(parameters, kSecAttrKeySizeInBits, (CFTypeRef)1024);

      SecKeyRef prsa = SecKeyGenerateSymmetric(parameters, NULL);

      if (prsa == NULL)
         return;

      CFRelease(parameters);

      return rsa;

   }

   void crypto::free_rsa_key(SecKeyRef prsa)
   {

      CFRelease(prsa);

   }


#elif defined(OPENSSL_CRYPTO)


   sp(::crypto::rsa) crypto::generate_rsa_key()
   {

      sp(::crypto::rsa) rsa = canew(::crypto::rsa(get_app()));

      RSA * & prsa = rsa->m_prsa;

      prsa = RSA_new();

      {

         BIGNUM * e = BN_new();

         BN_set_word(e, 65537);

         int ret = RSA_generate_key_ex(prsa, 1024, e, NULL);

         if (ret != 1)
         {

            BN_free(e);


            return NULL;

         }

         BN_free(e);

      }

      const BIGNUM * n = NULL;
      const BIGNUM * e = NULL;
      const BIGNUM * d = NULL;
      const BIGNUM * p = NULL;
      const BIGNUM * q = NULL;
      const BIGNUM * dmp1 = NULL;
      const BIGNUM * dmq1 = NULL;
      const BIGNUM * iqmp = NULL;

#if defined(METROWIN) || defined(LINUX)

      char * hexN       = BN_bn2hex(prsa->n);
      char * hexE       = BN_bn2hex(prsa->e);
      char * hexD       = BN_bn2hex(prsa->d);
      char * hexP       = BN_bn2hex(prsa->p);
      char * hexQ       = BN_bn2hex(prsa->q);
      char * hexDmp1    = BN_bn2hex(prsa->dmp1);
      char * hexDmq1    = BN_bn2hex(prsa->dmq1);
      char * hexIqmp    = BN_bn2hex(prsa->iqmp);

#else

      RSA_get0_key(prsa, &n, &e, &d);
      RSA_get0_factors(prsa, &p, &q);
      RSA_get0_crt_params(prsa, &dmp1, &dmq1, &iqmp);

      char * hexN = BN_bn2hex(n);
      char * hexE = BN_bn2hex(e);
      char * hexD = BN_bn2hex(d);
      char * hexP = BN_bn2hex(p);
      char * hexQ = BN_bn2hex(q);
      char * hexDmp1 = BN_bn2hex(dmp1);
      char * hexDmq1 = BN_bn2hex(dmq1);
      char * hexIqmp = BN_bn2hex(iqmp);

#endif

      rsa->n            = hexN;
      rsa->e            = hexE;
      rsa->d            = hexD;
      rsa->p            = hexP;
      rsa->q            = hexQ;
      rsa->dmp1         = hexDmp1;
      rsa->dmq1         = hexDmq1;
      rsa->iqmp         = hexIqmp;

      OPENSSL_free(hexN);
      OPENSSL_free(hexE);
      OPENSSL_free(hexD);
      OPENSSL_free(hexP);
      OPENSSL_free(hexQ);
      OPENSSL_free(hexDmp1);
      OPENSSL_free(hexDmq1);
      OPENSSL_free(hexIqmp);

      return rsa;

   }


   sp(rsa) crypto::read_priv_pem(const string & strFile)
   {
      FILE * f = fopen(strFile,"rb");
      sp(::crypto::rsa) rsa = canew(::crypto::rsa(get_app()));
      RSA * & prsa = rsa->m_prsa;
      PEM_read_RSAPrivateKey(f,&prsa,NULL,NULL);
      fclose(f);
      return rsa;
   }

   sp(rsa) crypto::read_pub_pem(const string & strFile)
   {
      FILE * f = fopen(strFile,"rb");
      sp(::crypto::rsa) rsa = canew(::crypto::rsa(get_app()));
      RSA * & prsa = rsa->m_prsa;
      PEM_read_RSA_PUBKEY(f,&prsa,NULL,NULL);
      fclose(f);
      return rsa;
   }



#else


   sp(::crypto::rsa) crypto::generate_rsa_key()
   {

      ::Windows::Security::Cryptography::Core::AsymmetricKeyAlgorithmProvider ^ provider =
         ::Windows::Security::Cryptography::Core::AsymmetricKeyAlgorithmProvider::OpenAlgorithm(
         ::Windows::Security::Cryptography::Core::AsymmetricAlgorithmNames::RsaPkcs1);


      return canew(::crypto::rsa(get_app(), provider->CreateKeyPair(1024)));

   }

   /*void crypto::free_rsa_key(sp(::crypto::rsa) prsa)
   {

      UNREFERENCED_PARAMETER(prsa);

   }*/

#endif

   void crypto::err_load_rsa_strings()
   {
#if defined(OPENSSL_CRYPTO)
      ERR_load_RSA_strings();

#endif

   }


   void crypto::err_load_crypto_strings()
   {

#if defined(OPENSSL_CRYPTO)

#if OPENSSL_API_COMPAT < 0x10100000L
      
      ERR_load_crypto_strings();
      
#else

//      OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);

#endif
      
#endif

   }


   rsa::rsa(::aura::application * papp) :
      ::object(papp),
      m_mutex(papp)
   {
#if defined(METROWIN)  && !defined(OPENSSL_CRYPTO)

      m_prsa = nullptr;
#else
      m_prsa = NULL;
#endif

   }


   rsa::rsa(::aura::application * papp, const string & nParam) :
      ::object(papp)
   {

#ifdef MACOS_DEPRECATED

      CFMutableDictionaryRef parameters = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

      CFDictionaryAddValue(parameters, kSecAttrKeyType, kSecAttrKeyTypeRSA);

      // not needed, defaults to true    CFDictionaryAddValue(parameters, kSecAttrCanEncrypt, kCFBooleanTrue);

      memory memKeyData;

      memKeyData.from_hex(strPass);

      CFDataRef keyData = memKeyData.get_os_cf_data();

      CFErrorRef error = NULL;

      m_prsa = SecKeyCreateFromData(parameters, keyData, &error);

      if (error != NULL)
      {

         CFRelease(parameters);

         CFRelease(keyData);

         CFRelease(error);

         throw resource_exception(get_app());

      }

      CFRelease(parameters);

      CFRelease(keyData);


#elif defined(METROWIN) && !defined(OPENSSL_CRYPTO)


      typedef struct _BCRYPT_RSAKEY_BLOB {
         ULONG Magic;
         ULONG BitLength;
         ULONG cbPublicExp;
         ULONG cbModulus;
         ULONG cbPrime1;
         ULONG cbPrime2;
      } BCRYPT_RSAKEY_BLOB;

      BCRYPT_RSAKEY_BLOB blob;

      ::memory_file memfile(get_app());

      //blob.Magic = 0x31415352; // BCRYPT_RSAPUBLIC_MAGIC;
      blob.Magic = 0x31415352; // BCRYPT_RSAPUBLIC_MAGIC;
      blob.BitLength = 1024;
      blob.cbPublicExp = 3;
      blob.cbModulus = 1024 / 8;
      blob.cbPrime1 = 0;
      blob.cbPrime2 = 0;

//      memory memVer(get_app());

  //    memVer.from_hex("00");

    //  memVer.prefix_der_uint();

      memfile.write(&blob, sizeof(blob));

      memory memMod(get_app());

      string strRsaModulus(nParam);

      memMod.from_hex(strRsaModulus);

      //memMod.reverse();



      //memMod.prefix_der_uint();

      memory memExp(get_app());

      memExp.from_hex("010001");

      //memExp.reverse();

      memfile.write(memExp.get_data(), memExp.get_size());

      memfile.write(memMod.get_data(), memMod.get_size());

      //memExp.prefix_der_uint();

      ::Windows::Security::Cryptography::Core::AsymmetricKeyAlgorithmProvider ^ cipher =
         ::Windows::Security::Cryptography::Core::AsymmetricKeyAlgorithmProvider::OpenAlgorithm(::Windows::Security::Cryptography::Core::AsymmetricAlgorithmNames::RsaPkcs1);


      //memory memKey(get_app());

      //memKey = memVer;
      //memKey += memMod;
      //memKey = memMod;
      //memKey += memExp;


      //memKey.prefix_der_sequence();

      //string strRsaPrivateKey = "-----BEGIN RSA PUBLIC KEY-----\r\n";
      //strRsaPrivateKey += chunk_split(System.base64().encode(memKey));
      //strRsaPrivateKey += "-----END RSA PUBLIC KEY-----";

      //memKey.allocate(strRsaPrivateKey.get_length());

      //memcpy(memKey.get_data(), strRsaPrivateKey, memKey.get_size());

      //m_prsa = cipher->ImportPublicKey(memKey.get_os_crypt_buffer(),::Windows::Security::Cryptography::Core::CryptographicPublicKeyBlobType::Pkcs1RsaPublicKey);
      m_prsa = cipher->ImportPublicKey(memfile.get_memory()->get_os_crypt_buffer(), ::Windows::Security::Cryptography::Core::CryptographicPublicKeyBlobType::BCryptPublicKey);

#else

      BIGNUM * n = BN_new();
      BIGNUM * e = BN_new();

      m_prsa = RSA_new();

      BN_hex2bn(&n, nParam);
      BN_hex2bn(&e, "10001");

#if defined(METROWIN) || defined(LINUX)
      m_prsa->n = n;
      m_prsa->e = e;
#else
      RSA_set0_key(m_prsa, n, e, NULL);
#endif

      //BN_free(n);
      //BN_free(e);


#endif

   }

#ifdef OPENSSL_CRYPTO

   rsa::rsa(::aura::application * papp,
      const string & strN,
      const string & strE,
      const string & strD,
      const string & strP,
      const string & strQ,
      const string & strDmp1,
      const string & strDmq1,
      const string & strIqmp) :
      ::object(papp),
      m_mutex(papp)
   {

      m_prsa = RSA_new();

      BIGNUM * n = BN_new();
      BIGNUM * e = BN_new();
      BIGNUM * d = BN_new();
      BIGNUM * p = BN_new();
      BIGNUM * q = BN_new();
      BIGNUM * dmp1 = BN_new();
      BIGNUM * dmq1 = BN_new();
      BIGNUM * iqmp = BN_new();

      BN_hex2bn(&n, strN);
      BN_hex2bn(&e, strE);
      BN_hex2bn(&d, strD);
      BN_hex2bn(&p, strP);
      BN_hex2bn(&q, strQ);
      BN_hex2bn(&dmp1, strDmp1);
      BN_hex2bn(&dmq1, strDmq1);
      BN_hex2bn(&iqmp, strIqmp);

#if defined(METROWIN) || defined(LINUX)
      m_prsa->n = n;
      m_prsa->e = e;
      m_prsa->d = d;
      m_prsa->p = p;
      m_prsa->q = q;
      m_prsa->dmp1 = dmp1;
      m_prsa->dmq1 = dmq1;
      m_prsa->iqmp = iqmp;
#else
      RSA_set0_key(m_prsa, n, e, d);
      RSA_set0_factors(m_prsa, p, q);
      RSA_set0_crt_params(m_prsa, dmp1, dmq1, iqmp);
#endif

   }

#endif

   rsa::~rsa()
   {

#ifdef MACOS_DEPRECATED
      if (m_prsa != NULL)
      {

         CFRelease(m_prsa);
         m_prsa = NULL;
      }
#elif defined(METROWIN) && !defined(OPENSSL_CRYPTO)

      m_prsa = nullptr;

#else
      if (m_prsa != NULL)
      {

         RSA_free(m_prsa);
         m_prsa = NULL;

      }
#endif

   }

   int rsa::public_encrypt(memory & out, const memory & in, string & strError)
   {
#ifdef MACOS_DEPRECATED


      SecTransformRef transform = SecEncryptTransformCreate(m_prsa, &error);

      if (error != NULL)
      {

         CFRelease(error);

         return "";

      }

      SecTransformSetAttribute(transform, kSecPaddingKey, kSecPaddingPKCS1Key, &error);

      if (error != NULL)
      {

         CFRelease(transform);

         CFRelease(error);

         return "";

      }

      memory memDataIn;

      memDataIn.from_hex(strRsaModulus);

      CFDataRef dataIn = memDataIn.get_os_cf_data();

      SecTransformSetAttribute(transform, kSecTransformInputAttributeName, dataIn, &error);

      if (error != NULL)
      {

         CFRelease(dataIn);

         CFRelease(transform);

         CFRelease(error);

         return "";

      }

      /* Encrypt the data. */

      CFDataRef data = (CFDataRef)SecTransformExecute(transform, &error);

      if (error != NULL)
      {

         CFRelease(dataIn);

         CFRelease(transform);

         CFRelease(error);

         return "";

      }


      string strHex;

      memory memory;

      memory.set_os_cf_data(data);

      memory.to_hex(strHex);

      CFRelease(data);

      CFRelease(dataIn);

      CFRelease(transform);

#elif defined(METROWIN) && !defined(OPENSSL_CRYPTO)



      out.set_os_crypt_buffer(::Windows::Security::Cryptography::Core::CryptographicEngine::Encrypt(m_prsa, in.get_os_crypt_buffer(), nullptr));




#else

      int32_t i = RSA_public_encrypt((int32_t)in.get_size(), (const uchar *)(const char *)in.get_data(), out.get_data(), m_prsa, RSA_PKCS1_PADDING);

      strError = ERR_error_string(ERR_get_error(), NULL);

      out.allocate(i);



#endif

      return out.get_size();


   }

   int rsa::private_decrypt(memory & out, const memory & in, string & strError)
   {


#if defined(METROWIN) && !defined(OPENSSL_CRYPTO)



out.set_os_crypt_buffer(::Windows::Security::Cryptography::Core::CryptographicEngine::Decrypt(m_prsa, in.get_os_crypt_buffer(), nullptr));




#else

      single_lock sl(&m_mutex, true);

      int32_t iRsaSize = 8192;

      out.allocate(iRsaSize);

      out.set(0);

      ::count i = RSA_private_decrypt((int)in.get_size(), in.get_data(), out.get_data(), m_prsa, RSA_PKCS1_PADDING);

      if (i < 0 || i >(1024 * 1024))
      {

         strError = ERR_error_string(ERR_get_error(), NULL);

         return (int)i;

      }

      out.allocate(i);

#endif

      return out.get_size();

   }

   int rsa::private_encrypt(memory & out,const memory & in,string & strError)
   {
#ifdef MACOS_DEPRECATED


      SecTransformRef transform = SecEncryptTransformCreate(m_prsa,&error);

      if(error != NULL)
      {

         CFRelease(error);

         return "";

      }

      SecTransformSetAttribute(transform,kSecPaddingKey,kSecPaddingPKCS1Key,&error);

      if(error != NULL)
      {

         CFRelease(transform);

         CFRelease(error);

         return "";

      }

      memory memDataIn;

      memDataIn.from_hex(strRsaModulus);

      CFDataRef dataIn = memDataIn.get_os_cf_data();

      SecTransformSetAttribute(transform,kSecTransformInputAttributeName,dataIn,&error);

      if(error != NULL)
      {

         CFRelease(dataIn);

         CFRelease(transform);

         CFRelease(error);

         return "";

      }

      /* Encrypt the data. */

      CFDataRef data = (CFDataRef)SecTransformExecute(transform,&error);

      if(error != NULL)
      {

         CFRelease(dataIn);

         CFRelease(transform);

         CFRelease(error);

         return "";

      }


      string strHex;

      memory memory;

      memory.set_os_cf_data(data);

      memory.to_hex(strHex);

      CFRelease(data);

      CFRelease(dataIn);

      CFRelease(transform);

#elif defined(METROWIN) && !defined(OPENSSL_CRYPTO)



      out.set_os_crypt_buffer(::Windows::Security::Cryptography::Core::CryptographicEngine::Encrypt(m_prsa,in.get_os_crypt_buffer(),nullptr));




#else

      int32_t i = RSA_private_encrypt((int32_t)in.get_size(),(const uchar *)(const char *)in.get_data(),out.get_data(),m_prsa,RSA_PKCS1_PADDING);

      strError = ERR_error_string(ERR_get_error(),NULL);

      out.allocate(i);



#endif

      return out.get_size();


   }

   int rsa::public_decrypt(memory & out,const memory & in,string & strError)
   {


#if defined(METROWIN) && !defined(OPENSSL_CRYPTO)



      out.set_os_crypt_buffer(::Windows::Security::Cryptography::Core::CryptographicEngine::Decrypt(m_prsa,in.get_os_crypt_buffer(),nullptr));




#else

      single_lock sl(&m_mutex,true);

      int32_t iRsaSize = 8192;

      out.allocate(iRsaSize);

      out.set(0);

      ::count i = RSA_public_decrypt((int)in.get_size(),in.get_data(),out.get_data(),m_prsa,RSA_PKCS1_PADDING);

      if(i < 0 || i >(1024 * 1024))
      {

         strError = ERR_error_string(ERR_get_error(),NULL);

         return (int)i;

      }

      out.allocate(i);

#endif

      return out.get_size();

   }


   string crypto::spa_login_crypt(const char * psz, const string & strRsa)
   {

      sp(::crypto::rsa) prsa = canew(::crypto::rsa(get_app(), strRsa));

      memory memory;

      ::memory memIn;

      memIn.from_hex(psz);

      memory.allocate(2048);

      memory.set(0);

      string strError;

      int i = prsa->public_encrypt(memory, memIn, strError);

      if (i < 0 || i >(1024 * 1024))
      {

         TRACE0(strError);

      }

      return memory.to_hex();

   }


   string crypto::spa_login_decrypt(const char * psz,const string & strRsa)
   {

      sp(::crypto::rsa) prsa = canew(::crypto::rsa(get_app(),strRsa));

      memory memory;

      ::memory memIn;

      memIn.from_hex(psz);

      memory.allocate(2048);

      memory.set(0);

      string strError;

      int i = prsa->private_decrypt(memory,memIn,strError);

      if(i < 0 || i >(1024 * 1024))
      {

         TRACE0(strError);

      }

      return memory.to_hex();

   }
   string crypto::spa_auth_decrypt(const char * psz,rsa * prsa)
   {

      memory memory;

      ::memory memIn;

      memIn.from_hex(psz);

      memory.allocate(2048);

      memory.set(0);

      string strError;

      int i = prsa->public_decrypt(memory,memIn,strError);

      if(i < 0 || i >(1024 * 1024))
      {

         TRACE0(strError);

      }

      return memory.to_hex();

   }

   string crypto::spa_auth_decrypt(const char * psz,const string & strRsa)
   {

      sp(::crypto::rsa) prsa = canew(::crypto::rsa(get_app(),strRsa));

      memory memory;

      ::memory memIn;

      memIn.from_hex(psz);

      memory.allocate(2048);

      memory.set(0);

      string strError;

      int i = prsa->public_decrypt(memory,memIn,strError);

      if(i < 0 || i >(1024 * 1024))
      {

         TRACE0(strError);

      }

      return memory.to_hex();

   }


   string crypto::spa_auth_crypt(const char * psz,rsa * prsa)
   {

      memory memory;

      ::memory memIn;

      memIn.from_hex(psz);

      memory.allocate(2048);

      memory.set(0);

      string strError;

      int i = prsa->private_encrypt(memory,memIn,strError);

      if(i < 0 || i >(1024 * 1024))
      {

         TRACE0(strError);

      }

      return memory.to_hex();

   }


   void crypto::np_make_zigbert_rsa(const string & strDir, const string & strSignerPath, const string & strKeyPath, const string & strOthersPath, const string & strSignature)
   {

#if !defined(METROWIN) && defined(OPENSSL_CRYPTO)

      X509 * signer = NULL;
      {
         string strSigner = Application.file().as_string(strSignerPath);
         BIO * pbio = BIO_new_mem_buf((void *)(LPCSTR)strSigner, (int32_t)strSigner.get_length());
         //signer = PEM_read_bio_X509_AUX(pbio, NULL, 0, NULL);
         signer = PEM_read_bio_X509(pbio, NULL, 0, NULL);
         BIO_free(pbio);
      }

      EVP_PKEY * pkey;
      {
         string strKey = Application.file().as_string(strKeyPath);
         BIO * pbio = BIO_new_mem_buf((void *)(LPCSTR)strKey, (int32_t)strKey.get_length());
         pkey = PEM_read_bio_PrivateKey(pbio, NULL, NULL, NULL);
         BIO_free(pbio);
      }


      stack_st_X509 * pstack509 = NULL;
      {
         string strOthers = Application.file().as_string(strOthersPath);
         ref_array < X509 > xptra;
         strsize iStart = 0;
         strsize iFind;
         string strEnd = "-----END CERTIFICATE-----";
         string strCertificate;
         strsize iEndLen = strEnd.get_length();
         ::count iCount = 0;
         while ((iFind = strOthers.find("-----BEGIN CERTIFICATE-----", iStart)) >= 0)
         {
            strsize iEnd = strOthers.find(strEnd, iFind);
            if (iEnd < 0)
               break;
            strCertificate = strOthers.Mid(iFind, iEnd + iEndLen - iFind);
            X509 * x;
            BIO * pbio = BIO_new(BIO_s_mem());
            BIO_puts(pbio, strCertificate);
            //x = PEM_read_bio_X509_AUX(pbio, NULL, 0, NULL);
            x = PEM_read_bio_X509(pbio, NULL, 0, NULL);
            BIO_free(pbio);
            if (x == NULL)
            {
               return;
            }
            xptra.add(x);
            iCount++;
            iStart = iEnd + iEndLen;
         }
         pstack509 = sk_X509_new_null();

         for (int32_t i = 0; i < xptra.get_count(); i++)
         {
            sk_X509_push(pstack509, xptra[i]);
         }
      }

      BIO * input = BIO_new_mem_buf((void *)(LPCSTR)strSignature, (int32_t)strSignature.get_length());

      PKCS7 * pkcs7 = PKCS7_sign(signer, pkey, pstack509, input, PKCS7_BINARY | PKCS7_DETACHED);

      BIO_free(input);
      sk_X509_free(pstack509);
      EVP_PKEY_free(pkey);
      X509_free(signer);

      BIO * output = BIO_new(BIO_s_mem());

      i2d_PKCS7_bio(output, pkcs7);

      char * pchData = NULL;
      long count = BIO_get_mem_data(output, &pchData);

      Application.file().put_contents(strDir / "META-INF/zigbert.rsa", pchData, count);

      BIO_free(output);
      PKCS7_free(pkcs7);
#endif
   }


   string crypto::txt_encrypt(const char * psz,rsa * prsa)
   {

      memory memory;

      ::memory memIn;

      memIn.assign(psz,strlen(psz));

      memory.allocate(2048);

      memory.set(0);

      string strError;

      int i = prsa->private_encrypt(memory,memIn,strError);

      if(i < 0 || i >(1024 * 1024))
      {

         TRACE0(strError);

      }

      return memory.to_hex();

   }


   string crypto::txt_decrypt(const char * psz,rsa * prsa)
   {

      memory memory;

      ::memory memIn;

      memIn.from_hex(psz);

      memory.allocate(2048);

      memory.set(0);

      string strError;

      int i = prsa->public_decrypt(memory,memIn,strError);

      if(i < 0 || i >(1024 * 1024))
      {

         TRACE0(strError);

      }

      return memory.to_string();

   }


} // namespace crypto




void
stunCalculateIntegrity_longterm(char* hmac, const char* input, int32_t length,
const char *username, const char *realm, const char *password)
{

#if !defined(METROWIN) || defined(OPENSSL_CRYPTO)
   uint32_t resultSize = 0;
   uchar HA1[16];
   char HA1_text[1024];

   snprintf(HA1_text, sizeof(HA1_text), "%s:%s:%s", username, realm, password);
   MD5((uchar *)HA1_text, strlen(HA1_text), HA1);

   HMAC(EVP_sha1(),
      HA1, 16,
      (const uchar*)input, length,
      (uchar*)hmac, &resultSize);
#endif
}

void
stunCalculateIntegrity_shortterm(char* hmac, const char* input, int32_t length, const char* key)
{
#if !defined(METROWIN) || defined(OPENSSL_CRYPTO)
   uint32_t resultSize = 0;
   HMAC(EVP_sha1(),
      key, (int)strlen(key),
      (const uchar*)input, length,
      (uchar*)hmac, &resultSize);
#endif
}

void hmac_evp_sha1_1234(unsigned char * hmac, unsigned int * hmacSize, const unsigned char * buf, size_t bufLen)
{
#if !defined(METROWIN) || defined(OPENSSL_CRYPTO)

   HMAC(EVP_sha1(),
      "1234", 4,
      (const uchar*)buf, bufLen - 20 - 4,
      hmac, hmacSize);
#endif

}

/*

bool crypt_file_get(const char * pszFile, string & str, const char * pszSalt)
{

string vsstr;

if(!crypt_file_get(pszFile, vsstr, pszSalt))
return false;

str = vsstr;

return true;

}


*/
