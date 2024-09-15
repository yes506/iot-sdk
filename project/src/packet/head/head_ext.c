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
#include "packet/head/head_ext.h"
#include "util/util.h"


/* =====================================
pHExt init
====================================== */
int im_headExt_init(IMHeadExtPtr pHExt) 
{
	pHExt->root_val = json_value_init_object();

	pHExt->root_obj = json_value_get_object(pHExt->root_val);
	pHExt->serialized_string = NULL;

	im_headExt_put_kvp_str(pHExt, "messageVersion", IOTMAKERS_EXT_MESSAGE_VER_STR);

	unsigned long long messageId = im_util_get_unique_number();
	char messageIdStr[32];
	snprintf(messageIdStr, 32, "%lld", messageId);
	im_headExt_put_kvp_str(pHExt, "messageId", messageIdStr);
	return 0;
}

int im_headExt_init_with_string(IMHeadExtPtr pHExt, char *json_str) 
{
	pHExt->root_val = json_parse_string(json_str);
	if ( pHExt->root_val == NULL )	{
		return -1;
	}
	pHExt->root_obj = json_value_get_object(pHExt->root_val);
	pHExt->serialized_string = NULL;
	return 0;
}

void im_headExt_release(IMHeadExtPtr pHExt) 
{
	if ( pHExt->serialized_string != NULL ){
		json_free_serialized_string(pHExt->serialized_string);
		pHExt->serialized_string = NULL;
	}

	if ( pHExt->root_val != NULL ){
		json_value_free(pHExt->root_val);
		pHExt->root_val = NULL;
	}
}


/* =====================================
key-value-pair
====================================== */
int im_headExt_put_kvp_str(IMHeadExtPtr pHExt, char *key, char *val) 
{
	pHExt->json_status = json_object_set_string(pHExt->root_obj, key, val);
	return (pHExt->json_status == JSONSuccess)?0:-1;
}
char* im_headExt_get_kvp_str(IMHeadExtPtr pHExt, char *key) 
{
	return (char*)json_object_get_string(pHExt->root_obj, key);
}
char* im_headExt_get_kvp_name_with_index(IMHeadExtPtr pHExt, int idx) 
{
	return (char*)json_object_get_name(pHExt->root_obj, idx);
}


/* =====================================
serialize
====================================== */
int im_packet_head_ext_get_serialized_strlen(IMHeadExtPtr pHExt) 
{
	//!!!
	//!!! json_serialization_size() counts the End Of String('\0') for string length.
	//!!!
	return json_serialization_size(pHExt->root_val) - 1;
}
char* im_packet_head_ext_get_serialized_str(IMHeadExtPtr pHExt) 
{
	if ( pHExt->serialized_string != NULL ){
		json_free_serialized_string(pHExt->serialized_string);
	}
	pHExt->serialized_string = json_serialize_to_string(pHExt->root_val);
	return (char*)pHExt->serialized_string;
}



/* =====================================
debug
====================================== */
void jsonwrap_print_value_serialized(JSON_Value* json_val)
{
    char *serialized_string = NULL;
	serialized_string = json_serialize_to_string_pretty(json_val);
	printf("serialized_string:\n%s\n", serialized_string);
    json_free_serialized_string(serialized_string);
}
