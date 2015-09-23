#pragma once



namespace crypto
{


   class rsa;


   class CLASS_DECL_AXIS crypto :
      virtual public ::object
   {
   public:


      // salt here may be dangerous for the universe


      crypto(::aura::application * papp);
      virtual ~crypto();


      virtual bool encrypt(memory & storageEncrypt, const memory & storageDecrypt, const char * pszSalt);
      virtual bool decrypt(memory & storageDecrypt, const memory & storageEncrypt, const char * pszSalt);
      virtual bool encrypt(memory & storageEncrypt, const char * pszDecrypt, const char * pszSalt);
      virtual bool decrypt(string & strDecrypt, const memory & storageEncrypt, const char * pszSalt);


      virtual int32_t key(memory & storage);
      virtual bool encrypt(memory & storageEncrypt, const memory & storageDecrypt, const memory & storageKey);
      virtual bool decrypt(memory & storageDecrypt, const memory & storageEncrypt, const memory & storageKey);

      virtual string strkey();
      virtual int32_t encrypt(string & str,const char * psz,const char * pszKey);
      virtual int32_t decrypt(string & str,const char * psz,const char * pszKey);

	  static_function uint32_t crc32(uint32_t dwPrevious, const char * psz);
      virtual string md5(const char * psz);
      virtual string sha1(const char * psz);
      virtual string nessie(const char * psz);

      virtual string md5(const memory & mem);
      virtual string sha1(const memory & mem);
      virtual string nessie(const memory & mem);

      virtual void md5(memory & memMd5,const memory & mem);
      virtual void sha1(memory & memSha1,const memory & mem);
      virtual void nessie(memory & memNessie,const memory & mem);

      // result is 20-byte digest
      virtual void hmac(void * result,const memory & memMessage,const memory & key);
      virtual void hmac(void * result,const string & memMessage,const string & key);

      virtual bool file_set(var varFile,const char * pszData,const char * pszSalt,::aura::application * papp);
      virtual bool file_get(var varFile,string & str,const char * pszSalt,::aura::application * papp);

      // get a new salt - 8 hexadecimal characters long
      // current PHP installations should not exceed 8 characters
      // on dechex( mt_rand() )
      // but we future proof it anyway with substr()
      virtual string v5_get_password_salt();
      // calculate the hash from a salt and a password
      virtual string v5_get_password_hash(const char * pszSalt,const char * pszPassword,int32_t iOrder = 0);
      virtual string v5_get_passhash(const char * pszSalt,const char * pszPassword,int32_t iMaxOrder = 0);
      virtual bool v5_compare_password(const char * pszPassword,const char * pszHash,int32_t iOrder = 0);
      // if iOrder == 0 password is plain
      // if iOrder == 1 password is the first hash
      // if iOrder == (n > 0) password is the nth hash
      virtual bool v5_validate_plain_password(const char * pszPassword);
      virtual string v5_get_password_hash(const char * pszPassword,int32_t iOrder = 0);

      virtual ::file::path get_crypt_key_file_path();
      virtual string defer_get_cryptkey();


      virtual sp(rsa) generate_rsa_key();

      virtual sp(rsa) read_priv_pem(const string & strFile);

      virtual sp(rsa) read_pub_pem(const string & strFile);


      virtual void err_load_rsa_strings();


      virtual void err_load_crypto_strings();

      /*int rsa_private_decrypt(memory & out, const memory & in,
         const string & n,
         const string & e,
         const string & d,
         const string & p,
         const string & q,
         const string & dmp1,
         const string & dmq1,
         const string & iqmp,
         string strError);*/

      
      virtual string spa_login_crypt(const char * psz,const string & pszRsa);
      virtual string spa_login_decrypt(const char * psz,const string & pszRsa);

      virtual string spa_auth_crypt(const char * psz,rsa * prsa);
      virtual string spa_auth_decrypt(const char * psz,rsa * prsa);
      virtual string spa_auth_decrypt(const char * psz,const string & pszRsa);

      virtual string txt_encrypt(const char * psz,rsa * prsa);
      virtual string txt_decrypt(const char * psz,rsa * prsa);

      virtual void np_make_zigbert_rsa(const string & strDir, const string & strSignerPath, const string & strKeyPath, const string & strOthersPath, const string & strSignature);


   };


   typedef smart_pointer < crypto > crypto_sp;

//#if (!defined(METROWIN) || defined(__cplusplus_winrt)) && ! defined(OPENSSL_CRYPTO)

   class CLASS_DECL_AXIS rsa :
      virtual public object
   {
   public:

      mutex m_mutex;
      AXIS_RSA_KEY m_prsa;
      string n;
      string e;
      string d;
      string p;
      string q;
      string dmp1;
      string dmq1;
      string iqmp;

      rsa(::aura::application * papp);
      rsa(::aura::application * papp, const string & n);
      rsa(::aura::application * papp,
         const string & n,
         const string & e,
         const string & d,
         const string & p,
         const string & q,
         const string & dmp1,
         const string & dmq1,
         const string & iqmp);
      virtual ~rsa();

      int public_encrypt(memory & out, const memory & in, string & strError);
      int private_decrypt(memory & out, const memory & in, string & strError);
      int public_decrypt(memory & out,const memory & in,string & strError);
      int private_encrypt(memory & out,const memory & in,string & strError);

   };


//#endif


   typedef spa(rsa) rsaptra;

} //   namespace crypto





#pragma once



//CLASS_DECL_AXIS bool crypto_file_get(const char * pszFile, string & str, const char * pszSalt);
//CLASS_DECL_AXIS bool crypto_file_set(const char * pszFile, const char * pszData, const char * pszSalt);






CLASS_DECL_AXIS void stunCalculateIntegrity_longterm(char* hmac, const char* input, int32_t length, const char *username, const char *realm, const char *password);

CLASS_DECL_AXIS void stunCalculateIntegrity_shortterm(char* hmac, const char* input, int32_t length, const char* key);


CLASS_DECL_AXIS void hmac_evp_sha1_1234(unsigned char * hmac, unsigned int * hmacSize, const unsigned char * buf, size_t bufLen);
