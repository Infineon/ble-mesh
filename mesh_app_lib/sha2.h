//
// - these files are from "https://github.com/h5p9sl/hmac_sha256.git [9445307885]
// - sha256.h and hmac_sha256.h were combined into this file
// - Additionally this convenience definition is added to avoid further changes in other files
#define hmac_sha256 sha2_hmac
// - there is no copyright in the original files, however, the
//   LICENSE file in that original git repo states:

/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <https://unlicense.org>
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  WjCryptLib_Sha256
//
//  Implementation of SHA256 hash function.
//  Original author: Tom St Denis, tomstdenis@gmail.com, http://libtom.org
//  Modified by WaterJuice retaining Public Domain license.
//
//  This is free and unencumbered software released into the public domain -
//  June 2013 waterjuice.org
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  IMPORTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <stdio.h>

typedef struct {
  uint64_t length;
  uint32_t state[8];
  uint32_t curlen;
  uint8_t buf[64];
} Sha256Context;

#define SHA256_HASH_SIZE (256 / 8)

typedef struct {
  uint8_t bytes[SHA256_HASH_SIZE];
} SHA256_HASH;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Sha256Initialise
//
//  Initialises a SHA256 Context. Use this to initialise/reset a context.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Sha256Initialise(Sha256Context* Context  // [out]
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Sha256Update
//
//  Adds data to the SHA256 context. This will process the data and update the
//  internal state of the context. Keep on calling this function until all the
//  data has been added. Then call Sha256Finalise to calculate the hash.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Sha256Update(Sha256Context* Context,  // [in out]
                  void const* Buffer,      // [in]
                  uint32_t BufferSize      // [in]
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Sha256Finalise
//
//  Performs the final calculation of the hash and returns the digest (32 byte
//  buffer containing 256bit hash). After calling this, Sha256Initialised must
//  be used to reuse the context.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Sha256Finalise(Sha256Context* Context,  // [in out]
                    SHA256_HASH* Digest      // [out]
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Sha256Calculate
//
//  Combines Sha256Initialise, Sha256Update, and Sha256Finalise into one
//  function. Calculates the SHA256 hash of the buffer.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Sha256Calculate(void const* Buffer,   // [in]
                     uint32_t BufferSize,  // [in]
                     SHA256_HASH* Digest   // [in]
);

/*
    hmac_sha256.h
    Originally written by https://github.com/h5p9sl
*/

#ifndef _HMAC_SHA256_H_
#define _HMAC_SHA256_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <stddef.h>

size_t  // Returns the number of bytes written to `out`
hmac_sha256(
    // [in]: The key and its length.
    //      Should be at least 32 bytes long for optimal security.
    const void* key,
    const size_t keylen,

    // [in]: The data to hash alongside the key.
    const void* data,
    const size_t datalen,

    // [out]: The output hash.
    //      Should be 32 bytes long. If it's less than 32 bytes,
    //      the resulting hash will be truncated to the specified length.
    void* out,
    const size_t outlen);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // _HMAC_SHA256_H_
