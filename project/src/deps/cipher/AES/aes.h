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



#ifndef _AES_H
#define _AES_H

#define uint8 	unsigned char
#define uint32	unsigned long int

#define NBITS				128 
#define BUFSIZE				256 

#define AES_MAXNR 			14
#define AES_BLOCK_SIZE 		16

#define BYTES_SIZE 			1024
#define KEY_SIZE 			128

#define GETU32(pt) (((u32)(pt)[0] << 24) ^ ((u32)(pt)[1] << 16) ^ ((u32)(pt)[2] <<  8) ^ ((u32)(pt)[3]))
#define PUTU32(ct, st) { (ct)[0] = (u8)((st) >> 24); (ct)[1] = (u8)((st) >> 16); (ct)[2] = (u8)((st) >>  8); (ct)[3] = (u8)(st); }

typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char u8;

#pragma pack(1)

/* This should be a hidden type, but EVP requires that the size be known */
struct aes_key_st {
    unsigned long rd_key[4 *(AES_MAXNR + 1)];
    int rounds;
};
typedef struct aes_key_st AES_KEY;

struct aes_context
{
    int		nr;				/* number of rounds */
    uint32	erk[64];		/* encryption round keys */
    uint32	drk[64];		/* decryption round keys */
};

struct ctr_state {
    unsigned char ivec[AES_BLOCK_SIZE];
    unsigned int num;
    unsigned char ecount[AES_BLOCK_SIZE];
};

// added for CBC & PKCS5 
struct aes_cbc_context
{
	struct	aes_context	ctx;
	uint8	iv[16];
	uint8	iv_prev[16];
};

#pragma pack()

int		aes_set_key( struct aes_context *ctx, uint8 *key, int nbits );
void	aes_encrypt( struct aes_context *ctx, uint8 data[16]);
void	aes_decrypt( struct aes_context *ctx, uint8 data[16]);
int		aes_enc_str( struct aes_context *ctx, uint8 *data, int size);
int		aes_dec_str( struct aes_context *ctx, uint8 *data, int size);
int		aes_enc_file( struct aes_context *ctx, char* in, char* out);
int		aes_dec_file( struct aes_context *ctx, char* in, char* out);

// added for PKCS5 padding
int		aes_encrypt_ecb_pkcs5 ( struct aes_context *ctx, const uint8 *plain, int plain_len, uint8* cipher, int cipher_len );
int		aes_decrypt_ecb_pkcs5 ( struct aes_context *ctx, const uint8 *cipher, int cipher_len, uint8* plain, int plain_len);


int		aes_set_key_cbc( struct aes_cbc_context *ctx, uint8 *key, uint8* iv);
int		aes_encrypt_cbc_pkcs5 ( struct aes_cbc_context *ctx, const uint8 *plain, int plain_len, uint8* cipher, int cipher_len );
int		aes_decrypt_cbc_pkcs5 ( struct aes_cbc_context *ctx, const uint8* cipher, int cipher_len, uint8 *plain, int plain_len );
int		aes_encrypt_cbc_pkcs5_file( struct aes_cbc_context *ctx, const char* plain_file, const char* cipher_file);
int		aes_decrypt_cbc_pkcs5_file( struct aes_cbc_context *ctx, const char* cipher_file, const char* plain_file);


int 	AES_set_encrypt_key(const unsigned char *userKey, const int bits, AES_KEY *key);
int 	AES_set_decrypt_key(const unsigned char *userKey, const int bits, AES_KEY *key);
void 	AES_encrypt(const unsigned char *in, unsigned char *out, const AES_KEY *key);
void 	AES_decrypt(const unsigned char *in, unsigned char *out, const AES_KEY *key);

void 	encrypt_ctr(unsigned char *key, unsigned char *indata,unsigned char *outdata ,int bytes_read);


#endif /* aes.h */
