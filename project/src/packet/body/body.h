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



#ifndef IOTMAKERS_PKTBODY_H
#define IOTMAKERS_PKTBODY_H

#include <stdio.h>
#include <string.h>
#include "parson/parson.h"
#include "packet/body/json_wrap.h"
#include "packet/body/body_util.h"
#include "util/util.h"


#if HAVE_MQTT
#define EXTR_IF_BODY_COMMON_deviceId	"devId"
#else
#define EXTR_IF_BODY_COMMON_deviceId	"deviceId"
#define EXTR_IF_BODY_COMMON_devId		"devId"
#endif

typedef struct {
  JSON_Status		json_status;
  char          *serialized_string;
  JSON_Value    *root;
} IMPacketBody, *IMPacketBodyPtr;


#ifdef __cplusplus
extern "C"
{
#endif

// body.c

int im_body_init(IMPacketBodyPtr body) ;
int im_body_init_with_string(IMPacketBodyPtr body, char *json_str) ;
void im_body_release(IMPacketBodyPtr body) ;
int im_body_set_strval(IMPacketBodyPtr body, char *key, char *val) ;
int im_body_set_numval(IMPacketBodyPtr body, char *key, double val) ;
char* im_body_get_strval(IMPacketBodyPtr body, char *key) ;
double im_body_get_numval(IMPacketBodyPtr body, char *key) ;
int im_body_get_value_bool(IMPacketBodyPtr body, char *key);

char* im_body_get_respCd(IMPacketBodyPtr body) ;
int im_body_get_respCd_as_int(IMPacketBodyPtr body) ;
char* im_body_get_respMsg(IMPacketBodyPtr body) ;
int im_body_get_serialized_strlen(IMPacketBodyPtr body) ;
char* im_body_get_serialized_string(IMPacketBodyPtr body) ;
int im_body_get_serialized_to_buff(IMPacketBodyPtr body, char *o_buff, int o_buff_len);
int im_body_get_deserialized_from_buff(IMPacketBodyPtr body, char *i_buff);
int im_body_get_deserialized_from_buff_len(IMPacketBodyPtr body, char *data, int data_len);
void im_body_print_serialized_string(IMPacketBodyPtr body);




// body_all_init.c
int im_body000_req_release(IMPacketBodyPtr body);
int im_body224_req_init(IMPacketBodyPtr body);
int im_body231_req_init(IMPacketBodyPtr body);
int im_body411_req_init(IMPacketBodyPtr body);
int im_body525_res_init(IMPacketBodyPtr body);
int im_body525_rep_init(IMPacketBodyPtr body);
int im_body711_res_one_init(IMPacketBodyPtr body);
int im_body711_res_all_init(IMPacketBodyPtr body);





// body_224_req.c
int im_body224_req_set_athnRqtNo(IMPacketBodyPtr body, char *dev_pw);
int im_body224sys_req_set_athnRqtNo(IMPacketBodyPtr body, char *dev_pw);

// body_411_req.c
int im_body411_req_append_resource_representation(IMPacketBodyPtr body, char* dev_id, char* resource_id, JSON_Value *j_properties_obj) ;
int im_body411sys_req_append_resource_representation(IMPacketBodyPtr body, char* sys_id, char* resource_id, JSON_Value *j_properties_obj);

///////////////////////////////////////////////////////////////
//
int im_body411_req_set_extrSysId(IMPacketBodyPtr body, char *dev_gw);
int im_body411_req_append_devColecData(IMPacketBodyPtr body, char* devId) ;
int im_body411_req_append_colecRow(IMPacketBodyPtr body, char *occDateStr, char *groupTagCd) ;



// body_121_res.c
char* im_body121_res_get_srvrDt(IMPacketBodyPtr body);
char* im_body121_res_get_srvrTimeZone(IMPacketBodyPtr body);

// body_224_req.c
int im_body224_req_set_extrSysId(IMPacketBodyPtr body, char *dev_gw);
int im_body224_req_set_devId(IMPacketBodyPtr body, char *dev_id);

// body_224_res.c
char* im_body224_res_body_get_athnRqtNo(IMPacketBodyPtr body);
char* im_body224_res_body_get_athnNo(IMPacketBodyPtr body);


// body_332_req.c
int im_body332_req_set_infoUpdTypeCd_insert(IMPacketBodyPtr body) ;
int im_body332_req_set_infoUpdTypeCd_delete(IMPacketBodyPtr body) ;
int im_body332_req_set_infoUpdTypeCd_update_all(IMPacketBodyPtr body) ;
int im_body332_req_set_infoUpdTypeCd_update(IMPacketBodyPtr body) ;
int im_body332_req_append_devBas(IMPacketBodyPtr body, char *extrSysId, char *devId);
int im_body332_req_set_string_data (IMPacketBodyPtr body, char *attr, char *string);
int im_body332_req_set_number_data (IMPacketBodyPtr body, char *attr, double number);


// body_411_res.c
char* im_body411_res_get_respCd(IMPacketBodyPtr body);

// body_525_rep.c
int im_body525_req_start_parse(IMPacketBodyPtr body) ;
int im_body525_req_get_resourceId (IMPacketBodyPtr body, char **o_resourceId);


// body_525_req.c
int im_body525_req_start_parse(IMPacketBodyPtr body) ;
int im_body525_req_next_devColecDataIdx(IMPacketBodyPtr body) ;
int im_body525_req_next_colecRowIdx(IMPacketBodyPtr body) ;
int im_body525_req_get_numdata_with_index (IMPacketBodyPtr body, int idx, char **o_devid, char **o_tagid, double *o_val);
int im_body525_req_get_strdata_with_index (IMPacketBodyPtr body, int idx, char **o_devid, char **o_tagid, char **o_val);

// body_525_res.c
char* im_body525_res_set_respCd(IMPacketBodyPtr body, char *val) ;
char* im_body525_res_set_respMsg(IMPacketBodyPtr body, char *val) ;

// body_711_req.c
int im_body711_req_start_parse(IMPacketBodyPtr body) ;
int im_body711_res_set_resource_representation(IMPacketBodyPtr body, char* resource_id, JSON_Value *j_properties_obj) ;
int im_body711_res_append_resource_representation(IMPacketBodyPtr body, char* resource_id, JSON_Value *j_properties_obj) ;


// body_731_req.c
int im_body731_req_set_extrSysId(IMPacketBodyPtr body, char* extrSysId) ;
int im_body731_req_set_devId(IMPacketBodyPtr body, char* devId) ;
int im_body731_req_set_seed(IMPacketBodyPtr body, char* seed) ;
int im_body731_req_set_otp(IMPacketBodyPtr body, char* otp) ;


// body_731_res.c
char* im_body731_res_get_otpRespCode(IMPacketBodyPtr body) ;
char* im_body731_res_get_otpRespMessage(IMPacketBodyPtr body) ;
char* im_body731_res_get_phoneNumber(IMPacketBodyPtr body) ;





#ifdef __cplusplus
}
#endif



#endif



