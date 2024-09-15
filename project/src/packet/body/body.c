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



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kt_iot_log.h"
#include "packet/body/json_wrap.h"
#include "packet/body/body.h"


/* =====================================
body init
====================================== */
int im_body_init(IMPacketBodyPtr body) 
{
	body->json_status = JSONSuccess;
	body->serialized_string = NULL;
	body->root = NULL;
	return 0;
}
int im_body_init_with_string(IMPacketBodyPtr body, char *json_str) 
{
	body->json_status = JSONSuccess;
	body->serialized_string = NULL;

	body->root = json_parse_string(json_str);
	if ( body->root == NULL )	{
		return -1;
	}
	return 0;
}
void im_body_release(IMPacketBodyPtr body) 
{
	if ( body->serialized_string != NULL ){
		json_free_serialized_string(body->serialized_string);
		body->serialized_string = NULL;
	}

	if ( body->root != NULL ){
		json_value_free(body->root);
		body->root = NULL;
	}
}

/* =====================================
	simple key-value-pair 
	2021-03-05
====================================== */
char* im_body_get_value_string(IMPacketBodyPtr body, char *key) 
{
	return (char*)jsonval_dotget_string(body->root, key);
}
double im_body_get_value_double(IMPacketBodyPtr body, char *key) 
{
	return (double)jsonval_dotget_number(body->root, key);
}
int im_body_get_value_integer(IMPacketBodyPtr body, char *key) 
{
	return (int)jsonval_dotget_number(body->root, key);
}
int im_body_get_value_bool(IMPacketBodyPtr body, char *key) 
{
	return (int)jsonval_dotget_bool(body->root, key);
}












/* =====================================
simple key-value-pair
====================================== */
int im_body_set_strval(IMPacketBodyPtr body, char *key, char *val) 
{
	return jsonval_dotset_string(body->root, key, val);
}
int im_body_set_numval(IMPacketBodyPtr body, char *key, double val) 
{
	return jsonval_dotset_number(body->root, key, val);
}


char* im_body_get_strval(IMPacketBodyPtr body, char *key) 
{
	return (char*)jsonval_dotget_string(body->root, key);
}
double im_body_get_numval(IMPacketBodyPtr body, char *key) 
{
	return (double)jsonval_dotget_number(body->root, key);
}

char* im_body_get_respCd(IMPacketBodyPtr body) 
{
	return im_body_get_strval(body, "respCd");
}

int im_body_get_respCd_as_int(IMPacketBodyPtr body) 
{
	char *respCd = im_body_get_strval(body, "respCd");
	if ( respCd == NULL )	{
		return -1;
	}
	return atoi(respCd);
}

char* im_body_get_respMsg(IMPacketBodyPtr body) 
{
	return im_body_get_strval(body, "respMsg");
}


/* =====================================
serialize
====================================== */
int im_body_get_serialized_strlen(IMPacketBodyPtr body) 
{
	//
	//!!! json_serialization_size() counts '\0' for string length.
	//
	return json_serialization_size(body->root) - 1;
}

char* im_body_get_serialized_string(IMPacketBodyPtr body) 
{
	if ( body->serialized_string != NULL ){
		json_free_serialized_string(body->serialized_string);
	}

	body->serialized_string = json_serialize_to_string(body->root);

	return (char*)body->serialized_string;
}

int im_body_get_serialized_to_buff(IMPacketBodyPtr body, char *o_buff, int o_buff_len)
{
	int len = im_body_get_serialized_strlen(body);
	if ( len <= 0 || o_buff_len < len )	{
		return -1;
	}

	strcpy(o_buff, im_body_get_serialized_string(body));

#if defined(CIPHER_AES128CBC)
    int cipher_len = 0;
    int cipher_buff_len = o_buff_len+32;
    char *cipher_buff = malloc(cipher_buff_len);
    if ( cipher_buff == NULL )    {
    	ERR("fail s\n", "malloc(cipher_buff)");
        return -1;
    }
	memset (cipher_buff, 0x00, sizeof(cipher_buff_len));

    cipher_len = aes_encrypt_cbc_pkcs5 (im_base_get_aes_cbc_ctx(), (unsigned char*)o_buff, len, 
        (unsigned char*)cipher_buff, cipher_buff_len);

    memcpy(o_buff, cipher_buff, cipher_len);
    free(cipher_buff);
	return cipher_len;
#else
	return len;
#endif
}

int im_body_get_deserialized_from_buff(IMPacketBodyPtr body, char *i_buff)
{
	// Just in case...
	im_body_release(body);
	return im_body_init_with_string(body, i_buff);
}

int im_body_get_deserialized_from_buff_len(IMPacketBodyPtr body, char *data, int data_len)
{
	// Just in case...
	im_body_release(body);

#if defined(CIPHER_AES128CBC)
    int plain_len = 0;
    int plain_buff_len = strlen(data);
    char *plain_buff = malloc(data_len);
    if ( plain_buff == NULL )    {
    	ERR("fail s\n", "malloc(plain_buff)");
        return -1;
    }
	memset (plain_buff, 0x00, sizeof(plain_buff_len));

    plain_len = aes_decrypt_cbc_pkcs5 (im_base_get_aes_cbc_ctx(), (unsigned char*)data, data_len, 
        (unsigned char*)plain_buff, plain_buff_len);

   	int rc = im_body_init_with_string(body, plain_buff);
    free(plain_buff);
	return rc;

#else

	if ( strlen(data) == 0 )	{
		return 0;
	}

	return im_body_init_with_string(body, data);
#endif
}



/* =====================================
debug
====================================== */
void im_body_print_serialized_string(IMPacketBodyPtr body)
{
    char *serialized_string = NULL;
	serialized_string = json_serialize_to_string_pretty(body->root);
	printf("serialized_string:\n%s\n", serialized_string);
    json_free_serialized_string(serialized_string);
}
