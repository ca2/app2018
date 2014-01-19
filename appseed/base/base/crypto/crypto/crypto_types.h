/*
 * crypto_types.h
 *
 * constants for cipher types and auth func types
 *
 * David A. McGrew
 * Cisco Systems, Inc.
 */
/*
 *   
 * Copyright(ca) 2001-2006 Cisco Systems, Inc.
 * 
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * 
 *   Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following
 *   disclaimer in the documentation and/or other materials provided
 *   with the distribution.
 * 
 *   Neither the name of the Cisco Systems, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#pragma once



#ifndef CRYPTO_TYPES_H
#define CRYPTO_TYPES_H

/**
 * @defgroup Algos Cryptographic Algorithms
 *
 *
 * This library provides several different cryptographic algorithms,
 * each of which can be selected by using the cipher_type_id_t and
 * auth_type_id_t.  These algorithms are documented below.
 *
 * Authentication functions that use the Universal Security Transform
 * (UST) must be used in conjunction with a cipher other than the NULL
 * cipher.  These functions require a per-message pseudorandom input
 * that is generated by the cipher.
 * 
 * The identifiers STRONGHOLD_AUTH and STRONGHOLD_CIPHER identify the
 * strongest available authentication function and cipher,
 * respectively.  They are resolved at compile time to the strongest
 * available algorithm.  The stronghold algorithms can serve as did
 * the keep of a medieval fortification; they provide the strongest
 * defense (or the last refuge).
 * 
 * @{
 */

/**
 * @defgroup Ciphers Cipher Types
 *
 * @brief    Each cipher type is identified by an uint32_t integer.  The
 *           cipher types available in this edition of libSRTP are given 
 *           by the #defines below.
 *
 * A cipher_type_id_t is an identifier for a cipher_type; only values
 * given by the #defines above (or those present in the file
 * crypto_types.h) should be used.
 *
 * The identifier STRONGHOLD_CIPHER indicates the strongest available
 * cipher, allowing an application to choose the strongest available
 * algorithm without any advance knowledge about the avaliable
 * algorithms.
 *
 * @{
 */

/**
 * @brief The NULL cipher performs no encryption.
 *
 * The NULL_CIPHER leaves its inputs unaltered, during both the 
 * encryption and decryption operations.  This cipher can be chosen
 * to indicate that no encryption is to be performed.
 */
#define NULL_CIPHER        0            

/** 
 * @brief AES-128 Integer Counter Mode (AES ICM)             
 *
 * AES-128 ICM is the variant of counter mode that is used by Secure RTP.  
 * This cipher uses a 16-octet key and a 30-octet offset (or salt) value.
 */
#define AES_128_ICM        1            

/**
 * @brief SEAL 3.0 
 * 
 * SEAL is the Software-Optimized Encryption Algorithm of Coppersmith
 * and Rogaway.  Nota bene: this cipher is IBM proprietary.
 */
#define SEAL               2            

/** 
 * @brief AES-128 Integer Counter Mode (AES ICM)             
 *
 * AES-128 ICM is the variant of counter mode that is used by Secure RTP.  
 * This cipher uses a 16-octet key and a 30-octet offset (or salt) value.
 */
#define AES_128_CBC        3            

/**
 * @brief Strongest available cipher.
 *
 * This identifier resolves to the strongest cipher type available.
 */
#define STRONGHOLD_CIPHER  AES_128_ICM  

/**
 * @}
 */



/**
 * @defgroup Authentication Authentication Function Types
 * 
 * @brief Each authentication function type is identified by an
 * uint32_t integer.  The authentication function types available in
 * this edition of libSRTP are given by the #defines below.
 *
 * An auth_type_id_t is an identifier for an authentication function type;
 * only values given by the #defines above (or those present in the 
 * file crypto_types.h) should be used.  
 *
 * The identifier STRONGHOLD_AUTH indicates the strongest available
 * authentication function, allowing an application to choose the
 * strongest available algorithm without any advance knowledge about
 * the avaliable algorithms.  The stronghold algorithms can serve as
 * did the keep of a medieval fortification; they provide the
 * strongest defense (or the last refuge).
 *
 * @{
 */

/**
 * @brief The NULL authentication function performs no authentication.
 *
 * The NULL_AUTH function does nothing, and can be selected to indicate
 * that authentication should not be performed.
 */ 
#define NULL_AUTH          0           

/**
 * @brief UST with TMMH Version 2
 *
 * UST_TMMHv2 implements the Truncated Multi-Modular Hash using
 * UST.  This function must be used in conjunction with a cipher other
 * than the NULL cipher.
 * with a cipher.
 */
#define UST_TMMHv2         1           

/**
 * @brief (UST) AES-128 XORMAC  
 *
 * UST_AES_128_XMAC implements AES-128 XORMAC, using UST. Nota bene:
 * the XORMAC algorithm is IBM proprietary.
 */
#define UST_AES_128_XMAC   2           

/**
 * @brief HMAC-SHA1
 *
 * HMAC_SHA1 implements the Hash-based MAC using the NIST Secure
 * Hash Algorithm version 1 (SHA1).
 */
#define HMAC_SHA1          3          

/**
 * @brief Strongest available authentication function.
 *
 * This identifier resolves to the strongest available authentication
 * function.
 */
#define STRONGHOLD_AUTH    HMAC_SHA1   

/**
 * @}
 */
/**
 * @}
 */

#endif  /* CRYPTO_TYPES_H */
