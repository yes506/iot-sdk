/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *  
 *  @brief IoTMakers Device packet; 디바이스 최종값조회 웅답
 *
 *  @section 샘플패킷

IF-711-rsp - serialized_string:
{
    "extrSysId": "HOME_IOT",
    "cmdDataInfoVOs": [{
        "dataTypeCd": "2502",
        "cmdData": []
    }],
    "devColecDataVOs": [{
        "devId": "W_085DDD27FB7507",
        "devTrtSttusCd": "01",
        "colecRowVOs": [{
            "occDt": "2016-06-0814:44:48.00",
            "binDataInfoVOs": [{
                "dataTypeCd": "2503",
                "binData": [0]
            }]
        }]
    }]
} 

 *
 */


#include "packet/body/body.h"



/**
{
	"id": "resource_id",
	"key": "val"
}
*/
int im_body711_res_set_resource_representation(IMPacketBodyPtr body, char* resource_id, JSON_Value *j_properties_obj) 
{
	jsonval_dotset_string(j_properties_obj, "id", resource_id);
	body->root = json_value_deep_copy(j_properties_obj);
	return 0;
}





/**
[
	{
		"id": "resource_id",
		"key": "val"
	}
	,
	{
		"id": "resource_id",
		"key": "val"
	}
]


*/
int im_body711_res_append_resource_representation(IMPacketBodyPtr body, char* resource_id, JSON_Value *j_properties_obj) 
{
	jsonval_dotset_string(j_properties_obj, "id", resource_id);
    jsonarr_append_value(json_value_get_array(body->root), json_value_deep_copy(j_properties_obj));
	return 0;
}




