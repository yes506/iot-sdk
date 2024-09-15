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



#ifndef IOTMAKERS_PKTHEAD_EXT_H
#define IOTMAKERS_PKTHEAD_EXT_H

#include "parson/parson.h"


#define IOTMAKERS_EXT_MESSAGE_VER_STR		"1.0"

typedef struct {
	JSON_Status json_status;
    JSON_Value	*root_val;
    JSON_Object *root_obj;

    char *serialized_string;
} IMHeadExt, *IMHeadExtPtr;

#ifdef __cplusplus
extern "C"
{
#endif

int im_headExt_init(IMHeadExtPtr pHExt) ;
int im_headExt_init_with_string(IMHeadExtPtr pHExt, char *json_str) ;
void im_headExt_release(IMHeadExtPtr pHExt) ;
int im_headExt_put_kvp_str(IMHeadExtPtr pHExt, char *key, char *val) ;
char* im_headExt_get_kvp_str(IMHeadExtPtr pHExt, char *key) ;
char* im_headExt_get_kvp_name_with_index(IMHeadExtPtr pHExt, int idx) ;
int im_packet_head_ext_get_serialized_strlen(IMHeadExtPtr pHExt) ;
char* im_packet_head_ext_get_serialized_str(IMHeadExtPtr pHExt) ;
void jsonwrap_print_value_serialized(JSON_Value* json_val);

JSON_Status json_object_set_string(JSON_Object *object, const char *name, const char *string);


#ifdef __cplusplus
}
#endif



#endif



