#ifndef HEADER_SHA_H
#define HEADER_SHA_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint8_t     hash[32];       // Changed by RKW, unsigned char becomes uint8_t
    uint32_t    buffer[16];     // Changed by RKW, unsigned long becomes uint32_t
    uint32_t    state[8];       // Changed by RKW, unsinged long becomes uint32_t
    uint8_t     length[8];      // Changed by RKW, unsigned char becomes uint8_t
} sha256;

typedef struct _hmac_sha256 {
    uint8_t     digest[32];     // Changed by RKW, unsigned char becomes uint_8
    uint8_t     key[64];        // Changed by RKW, unsigned char becomes uint_8
    sha256      sha;
} hmac_sha256;

void sha256_initialize(sha256 *sha);
void sha256_update(sha256 *sha, const uint8_t *message, uint32_t length);
void sha256_finalize(sha256 *sha, const uint8_t *message, uint32_t length);
void sha256_get(uint8_t hash[32], const uint8_t *message, int length);
void hmac_sha256_initialize(hmac_sha256 *hmac, const uint8_t *key, int length);
void hmac_sha256_update(hmac_sha256 *hmac, const uint8_t *message, int length);
void hmac_sha256_finalize(hmac_sha256 *hmac, const uint8_t *message, int length);
void hmac_sha256_get(uint8_t digest[32], const uint8_t *message, int message_length, const uint8_t *key, int key_length);
int hmac_sha256_generate(unsigned char *key, int key_len, unsigned char *data, int data_len, unsigned char *digest, int digest_len,unsigned char *result);

#endif
