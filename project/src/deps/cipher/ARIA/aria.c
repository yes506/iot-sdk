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

 
#include "aria.h"


/************************************************************************
    2017-12-26 kcshin @ KT
*/
static void do_pad_remain_pkcs7( uint8_t *dest, uint8_t *src, int remainder)
{
  int i;
#if 0 // 2020-01-23 16바이트 패딩시에 0이 아니라 16으로 패딩해야함.
  if (remainder == 0) {
    	memset(dest, 0x00, ARIA_BLOCK_SIZE);
	} else {
		memcpy(dest, src, remainder);
    for (i = remainder; i < ARIA_BLOCK_SIZE; i++) {
            dest[i] = ARIA_BLOCK_SIZE - remainder;
    }
  }
#else
    uint8_t remain = ARIA_BLOCK_SIZE - remainder;
    memcpy(dest, src, remainder);
    for (i = remainder; i < ARIA_BLOCK_SIZE; i++) {
            dest[i] = remain;
    }
#endif

}

static int do_unpad_remain_pkcs7( uint8_t *dest)
{
  int i;
	int padding = dest[ARIA_BLOCK_SIZE-1];
  if (padding < 1 || padding > ARIA_BLOCK_SIZE)
        return -1;

  for ( i = 1; i <= padding; ++i)    {
    if (padding != dest[ARIA_BLOCK_SIZE-i])
      return -1;
  }
  memset (dest+(ARIA_BLOCK_SIZE-padding), 0x00, padding);

  return padding;
}




/****************************************************
    2017-12-27
    kcshin@KT
*/
static int	ariacore_set_key128( AriaCoreContext *ctx, uint8_t *key)
{
    memcpy(ctx->mk, key, 16);
    ctx->nr = AriaCore_EncKeySetup(ctx->mk, ctx->erk, 128);
    AriaCore_DecKeySetup(ctx->mk, ctx->drk, 128);
    return ctx->nr;
}

static int	ariacore_set_key192( AriaCoreContext *ctx, uint8_t *key)
{
    memcpy(ctx->mk, key, 24);
    ctx->nr = AriaCore_EncKeySetup(ctx->mk, ctx->erk, 192);
    AriaCore_DecKeySetup(ctx->mk, ctx->drk, 192);
    return ctx->nr;
}

static int	ariacore_set_key256( AriaCoreContext *ctx, uint8_t *key)
{
    memcpy(ctx->mk, key, 32);
    ctx->nr = AriaCore_EncKeySetup(ctx->mk, ctx->erk, 256);
    AriaCore_DecKeySetup(ctx->mk, ctx->drk, 256);
    return ctx->nr;
}

static int ariacore_encrypt( AriaCoreContext *ctx, uint8_t *in, uint8_t *out)
{
    AriaCore_Crypt(in, ctx->nr, ctx->erk, out);
    return 0;
}


static int	ariacore_decrypt( AriaCoreContext *ctx, uint8_t *in, uint8_t *out)
{
    AriaCore_Crypt(in, ctx->nr, ctx->drk, out);
    return 0;
}




/************************************************************************
    NO Block Chain
*/
int	aria_set_key128( AriaContext *ctx, uint8_t *key)
{
    return ariacore_set_key128(&(ctx->ctx), key);    
}
int	aria_set_key192( AriaContext *ctx, uint8_t *key)
{
    return ariacore_set_key192(&(ctx->ctx), key);    
}
int	aria_set_key256( AriaContext *ctx, uint8_t *key)
{
    return ariacore_set_key256(&(ctx->ctx), key);    
}
int	aria_set_key( AriaContext *ctx, uint8_t *key)
{
    return aria_set_key128(ctx, key);
}

static void aria_encrypt( AriaContext *ctx, uint8_t data[16])
{
    ariacore_encrypt(&(ctx->ctx), data, data);
}
static void aria_decrypt( AriaContext *ctx, uint8_t data[16])
{
    ariacore_decrypt(&(ctx->ctx), data, data);
}


/**
    NO Block Chain  & PKCS5
*/
int	aria_encrypt_pkcs5( AriaContext *ctx, const uint8_t *plain, int plain_len, uint8_t* cipher, int cipher_len )
{
	int i, j, quotient, remainder, cipher_len_need;;
	uint8_t tmpData[ARIA_BLOCK_SIZE];

	quotient = (int) (plain_len / ARIA_BLOCK_SIZE);
	remainder = plain_len % ARIA_BLOCK_SIZE;

    cipher_len_need = (quotient + 1) * ARIA_BLOCK_SIZE;
    if ( cipher_len < cipher_len_need )
			return -1;

    // encrypt blocks before last one.
    for ( i = 0; i < quotient; i++ ) {
    	memcpy(tmpData, (uint8_t *) (plain + (i * ARIA_BLOCK_SIZE)), ARIA_BLOCK_SIZE);
		aria_encrypt(ctx, tmpData);
		memcpy((uint8_t *) (cipher + (i * ARIA_BLOCK_SIZE)), tmpData, ARIA_BLOCK_SIZE);
    }

    // pad the last block in pkcs5.
    do_pad_remain_pkcs7(tmpData, (uint8_t *) (plain + (i * ARIA_BLOCK_SIZE)), remainder);

    // encrypt the last block.
    aria_encrypt(ctx, tmpData);
    memcpy((uint8_t *) (cipher + (i * ARIA_BLOCK_SIZE)), tmpData, ARIA_BLOCK_SIZE);

	return cipher_len_need;
}


int	aria_decrypt_pkcs5 ( AriaContext *ctx, const uint8_t *cipher, int cipher_len, uint8_t* plain, int plain_len)
{
	int i, j, quotient, remainder, padding;
	uint8_t  tmpData[ARIA_BLOCK_SIZE];

	quotient = (int) (cipher_len / ARIA_BLOCK_SIZE);
	remainder = cipher_len % ARIA_BLOCK_SIZE;
	if (remainder != 0 || cipher_len < ARIA_BLOCK_SIZE )
		return -1;

	if (plain_len < (quotient*ARIA_BLOCK_SIZE) )
		return -1;

    // decrypt blocks before last one.
    for ( i = 0; i < quotient-1; i++) {
		memcpy(tmpData, (uint8_t *) (cipher + (i * ARIA_BLOCK_SIZE)), ARIA_BLOCK_SIZE);
        aria_decrypt(ctx, tmpData);
		memcpy((uint8_t *) (plain + (i * ARIA_BLOCK_SIZE)), tmpData, ARIA_BLOCK_SIZE);
	}

    // decrypt the last block.
	memcpy(tmpData, (uint8_t *) (cipher + (i * ARIA_BLOCK_SIZE)), ARIA_BLOCK_SIZE);
    aria_decrypt(ctx, tmpData);

    // unpad the last block in pkcs5.
    if ( (padding = do_unpad_remain_pkcs7(tmpData)) < 0 )    {
        return -1;
    }
    do_unpad_remain_pkcs7(tmpData);

    memcpy((uint8_t *) (plain + (i * ARIA_BLOCK_SIZE)), tmpData, ARIA_BLOCK_SIZE);

	return cipher_len - padding;
}

/************************************************************************
    CBC & PKCS5
*/

int	aria_set_key128_cbc( AriaContext *ctx, uint8_t *key, uint8_t *iv)
{
	memset (ctx->iv, 0x00, sizeof(ctx->iv));
	if (NULL != iv)
		memcpy (ctx->iv, iv, sizeof(ctx->iv));

    return ariacore_set_key128(&(ctx->ctx), key);    
}
int	aria_set_key192_cbc( AriaContext *ctx, uint8_t *key, uint8_t *iv)
{
	memset (ctx->iv, 0x00, sizeof(ctx->iv));
	if (NULL != iv)
		memcpy (ctx->iv, iv, sizeof(ctx->iv));

    return ariacore_set_key192(&(ctx->ctx), key);    
}
int	aria_set_key256_cbc( AriaContext *ctx, uint8_t *key, uint8_t *iv)
{
	memset (ctx->iv, 0x00, sizeof(ctx->iv));
	if (NULL != iv)
		memcpy (ctx->iv, iv, sizeof(ctx->iv));

    return ariacore_set_key256(&(ctx->ctx), key);    
}
int	aria_set_key_cbc( AriaContext *ctx, uint8_t *key, uint8_t *iv)
{
    return aria_set_key128_cbc(ctx, key, iv);
}

/**
 * CBC
 */
static void aria_encrypt_cbc( AriaContext *ctx, uint8_t data[16])
{
    int j;
    uint8_t tmpCipher[ARIA_BLOCK_SIZE];

	for (j=0; j<16; ++j)	{
		data[j] ^= ctx->iv_prev[j];
	}

    ariacore_encrypt(&(ctx->ctx), data, data);
	
    memcpy(ctx->iv_prev, data, 16);
}

static void aria_decrypt_cbc( AriaContext *ctx, uint8_t data[16])
{
	int j;
	uint8_t tmpData[16];

	memcpy (tmpData, data, 16);

	ariacore_decrypt ( &(ctx->ctx), data, data);

	for (j=0; j<16; ++j)	{
		data[j] ^= ctx->iv_prev[j];
	}
	memcpy(ctx->iv_prev, tmpData, 16);
}



/**
 * CBC & PKCS5
 */


int	aria_encrypt_cbc_pkcs5( AriaContext *ctx, const uint8_t *plain, int plain_len, uint8_t* cipher, int cipher_len )
{
	int i, j, quotient, remainder, cipher_len_need;
	uint8_t tmpData[ARIA_BLOCK_SIZE];

	quotient = (int) (plain_len / ARIA_BLOCK_SIZE);
	remainder = plain_len % ARIA_BLOCK_SIZE;

    cipher_len_need = (quotient + 1) * ARIA_BLOCK_SIZE;
    if ( cipher_len < cipher_len_need )
			return -1;

    // set initial vector for CBC
	memcpy (ctx->iv_prev, ctx->iv, sizeof(ctx->iv_prev));

    // encrypt blocks before last one.
    for ( i = 0; i < quotient; i++ ) {
    	memcpy(tmpData, (uint8_t *) (plain + (i * ARIA_BLOCK_SIZE)), ARIA_BLOCK_SIZE);
		aria_encrypt_cbc(ctx, tmpData);
		memcpy((uint8_t *) (cipher + (i * ARIA_BLOCK_SIZE)), tmpData, ARIA_BLOCK_SIZE);
    }

    // pad the last block in pkcs5.
    do_pad_remain_pkcs7(tmpData, (uint8_t *) (plain + (i * ARIA_BLOCK_SIZE)), remainder);

    // encrypt the last block.
    aria_encrypt_cbc(ctx, tmpData);
    memcpy((uint8_t *) (cipher + (i * ARIA_BLOCK_SIZE)), tmpData, ARIA_BLOCK_SIZE);

	return cipher_len_need;
}

int	aria_decrypt_cbc_pkcs5 ( AriaContext *ctx, const uint8_t *cipher, int cipher_len, uint8_t* plain, int plain_len)
{
	int i, j, quotient, remainder, padding;
	uint8_t  tmpData[ARIA_BLOCK_SIZE];

	quotient = (int) (cipher_len / ARIA_BLOCK_SIZE);
	remainder = cipher_len % ARIA_BLOCK_SIZE;
	if (remainder != 0 || cipher_len < ARIA_BLOCK_SIZE )
		return -1;

	if (plain_len < (quotient*ARIA_BLOCK_SIZE) )
		return -1;

    // set initial vector for CBC
	memcpy (ctx->iv_prev, ctx->iv, sizeof(ctx->iv_prev));

    // decrypt blocks before last one.
    for ( i = 0; i < quotient-1; i++) {
		memcpy(tmpData, (uint8_t *) (cipher + (i * ARIA_BLOCK_SIZE)), ARIA_BLOCK_SIZE);
        aria_decrypt_cbc(ctx, tmpData);
		memcpy((uint8_t *) (plain + (i * ARIA_BLOCK_SIZE)), tmpData, ARIA_BLOCK_SIZE);
	}

    // decrypt the last block.
	memcpy(tmpData, (uint8_t *) (cipher + (i * ARIA_BLOCK_SIZE)), ARIA_BLOCK_SIZE);
    aria_decrypt_cbc(ctx, tmpData);

    // unpad the last block in pkcs5.
    if ( (padding = do_unpad_remain_pkcs7(tmpData)) < 0 )    {
        return -1;
    }
    do_unpad_remain_pkcs7(tmpData);

    memcpy((uint8_t *) (plain + (i * ARIA_BLOCK_SIZE)), tmpData, ARIA_BLOCK_SIZE-padding);

	return cipher_len - padding;
}
