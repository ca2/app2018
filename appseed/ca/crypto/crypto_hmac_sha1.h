#pragma once


namespace crypto
{


   namespace hmac_sha1
   {


      class CLASS_DECL_ca context
      {
      public:

         // digest is 20-byte digest
         void digest(void * digest, const void * text, int text_len, const void * key, int key_len);
   
      };


   } // namespace hmac_sha1


} // namespace crypto


