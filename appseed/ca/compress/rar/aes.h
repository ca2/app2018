// Crypto/RarAes.h
// from 7-zip
#pragma once

#define AES_BLOCK_SIZE 16

namespace crypto
{
   
   namespace rar29
   {

      const uint32_t kRarAesKeySize = 16;

      class decoder:
        public aes_cbc_decoder,
        public compress::set_decoder_properties2_interface,
        public crypto::set_password_interface
      {
      public:

         byte               _salt[8];
         bool               _thereIsSalt;
         ex1::byte_buffer   buffer;
         byte               aesKey[kRarAesKeySize];
         byte               _aesInit[AES_BLOCK_SIZE];
         bool               _needCalculate;
         bool               _rar350Mode;

         void Calculate();
         ex1::HRes Init();
         ex1::HRes CryptoSetPassword(const byte *aData, uint32_t aSize);
         ex1::HRes SetDecoderProperties2(const byte *data, uint32_t size);

         decoder();
         void SetRar350Mode(bool rar350Mode) { _rar350Mode = rar350Mode; }
      };

   } // namespace rar29

} // namespace crypto


