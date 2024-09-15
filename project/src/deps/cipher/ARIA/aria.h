/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *
 */


#ifndef _ARIA_H
#define _ARIA_H

#include <stdint.h>
#include <inttypes.h>
#include <stddef.h>
#include <string.h>

//ARIA block size
#define ARIA_BLOCK_SIZE 16

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief ARIA algorithm context
 **/

typedef struct
{
    int nr;
    uint8_t mk[32];
    uint8_t erk[16*17];
    uint8_t drk[16*17];
} AriaCoreContext;


/** 2017-12-26 kcshin
 *  added for CBC & PKCS5 
 */
typedef struct
{
    AriaCoreContext ctx;
	
    uint8_t	iv[16];         // CBC
	uint8_t	iv_prev[16];    // CBC
} AriaContext;

//ARIA Core related functions

int AriaCore_EncKeySetup(const uint8_t *w0, uint8_t *e, int keyBits) ;
int AriaCore_DecKeySetup(const uint8_t *w0, uint8_t *d, int keyBits);
void AriaCore_Crypt(const uint8_t *p, int R, const uint8_t *e, uint8_t *c);


//ARIA related functions

int	aria_set_key128( AriaContext *ctx, uint8_t *key);
int	aria_set_key192( AriaContext *ctx, uint8_t *key);
int	aria_set_key256( AriaContext *ctx, uint8_t *key);
int	aria_set_key( AriaContext *ctx, uint8_t *key);
int	aria_encrypt_pkcs5( AriaContext *ctx, const uint8_t *plain, int plain_len, uint8_t* cipher, int cipher_len );
int	aria_decrypt_pkcs5 ( AriaContext *ctx, const uint8_t *cipher, int cipher_len, uint8_t* plain, int plain_len);


int	aria_set_key128_cbc( AriaContext *ctx, uint8_t *key, uint8_t *iv);
int	aria_set_key192_cbc( AriaContext *ctx, uint8_t *key, uint8_t *iv);
int	aria_set_key256_cbc( AriaContext *ctx, uint8_t *key, uint8_t *iv);
int	aria_set_key_cbc( AriaContext *ctx, uint8_t *key, uint8_t *iv);
int	aria_encrypt_cbc_pkcs5( AriaContext *ctx, const uint8_t *plain, int plain_len, uint8_t* cipher, int cipher_len );
int	aria_decrypt_cbc_pkcs5 ( AriaContext *ctx, const uint8_t *cipher, int cipher_len, uint8_t* plain, int plain_len);



//C++ guard
#ifdef __cplusplus
   }
#endif

#endif