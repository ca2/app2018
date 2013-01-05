/*
 * null-cipher.h
 *
 * header file for the null cipher
 *
 *
 * David A. McGrew
 * Cisco Systems, Inc.
 */

/*
 *   
 * Copyright (c) 2001-2006, Cisco Systems, Inc.
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




#ifndef NULL_CIPHER_H
#define NULL_CIPHER_H


typedef struct {
  char foo ;/* is_empty, for now */
} null_cipher_ctx_t;


/*
 * none of these functions do anything (though future versions may keep
 * track of bytes encrypted, number of instances, and/or other info).
 */

err_status_t
null_cipher_init(null_cipher_ctx_t *c, const uint8_t *key);

err_status_t
null_cipher_set_segment(null_cipher_ctx_t *c,
         uint32_t index);

err_status_t
null_cipher_encrypt(null_cipher_ctx_t *c,
          uchar *buf, uint32_t *bytes_to_encr);


err_status_t
null_cipher_encrypt_aligned(null_cipher_ctx_t *c,
             uchar *buf, int32_t bytes_to_encr);

#endif /* NULL_CIPHER_H */
