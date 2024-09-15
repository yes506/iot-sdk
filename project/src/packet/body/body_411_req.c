/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *  
 *  @brief IoTMakers Device packet; 데이터수집 요청
 *
 *  @section 샘플패킷

// DEVICE
///////////////////////////////////////////////
[
{
"subscriptionReference": "/devices/{device_id}/subscriptios/subscribeAll",
"eventReference": "/devices/{device_id}/resource/{resource_id}",
"eventType": "UPDATED",
"eventTime": "2022-04-04T13:00:00:123",
"representation": {
	"{key}": {value}
	}
}
]
///////////////////////////////////////////////////////////
[
	{
		"subscriptionReference": "/devices/testbyw111/subscriptions/subscriptionAll",
		"eventReference": "/devices/testbyw111/resources/testAllTypeUri",
		"representation": {
			"ectestBoolean": true,
			"ectestDoubleArray": [
				10.1,
				20.2
			],
			"ectestObject": {
				"x": 10,
				"y": 20,
				"z": 30
			},
			"ectestString": "testValue",
			"ectestDouble": 10.1,
			"ectestBooleanArray": [
				true,
				false
			],
			"ectestIntegerArray": [
				10,
				20
			],
			"ectestStringArray": [
				"testValue1",
				"testValue2"
			],
			"id": "testAllTypeUri",
			"ectestObjectArray": [
				{
					"a": 10,
					"b": 20,
					"c": 30
				},
				{
					"x": "10",
					"y": "20",
					"z": "30"
				}
			],
			"ectestBin": [
				97,
				98,
				99,
				100
			],
			"ectestInteger": 10
		},
		"eventTime": "2021-03-03T17:55:19.507"
	}
]



///////////////////////////////////////
// SYSTEM
   - 가변 Header : 
   
{
	"messageVersion": "1.0",
	"messageId": "1617000993810",
	"operation": "NOTIFY",
	"to": "/systems/notifications"
}
   - Body : 
   
[
	{
		"subscriptionReference": "/systems/testbyw11/subscriptions/subscriptionAll",
		"eventReference": "/systems/testbyw11/resources/testAllTypeUri",
		"representation": {
			"ectestBoolean": true,
			"ectestDoubleArray": [
				10.1,
				20.2
			],
			"ectestObject": {
				"x": 10
			},
			"ectestString": "testValue",
			"ectestDouble": 10.1,
			"ectestBooleanArray": [
				true,
				false
			],
			"ectestIntegerArray": [
				10,
				20
			],
			"ectestStringArray": [
				"testValue1",
				"testValue2"
			],
			"id": "testAllTypeUri",
			"ectestObjectArray": [
				{
					"x": 10,
					"y": 20,
					"z": 30
				},
				{
					"x": 10,
					"y": 20,
					"z": 30
				}
			],
			"ectestBin": [
				97,
				98,
				99,
				100
			],
			"ectestInteger": 10
		},
		"eventTime": "2021-03-29T15:56:33.810"
	}
]


 *
 */
#include "packet/body/body.h"


static JSON_Value *g_json_representation = NULL;


static void __build_resource_item_common(JSON_Value *json_val, char* dev_id, char* resource_id)
{
	char buff[128];

	sprintf(buff, "/devices/%s/subscriptions/subscribeAll", dev_id);
	jsonval_dotset_string(json_val, "subscriptionReference", buff);

	sprintf(buff, "/devices/%s/resource/%s", dev_id, resource_id);
	jsonval_dotset_string(json_val, "eventReference", buff);
	
	jsonval_dotset_string(json_val, "eventType", "UPDATED");

    im_util_strftime_now(buff, "%Y-%m-%dT%H:%M:%S.");
    sprintf(buff+strlen(buff), "%03d", im_util_gettime_now_msec());
	jsonval_dotset_string(json_val, "eventTime", buff);

	return;
}


int im_body411_req_append_resource_representation(IMPacketBodyPtr body, char* dev_id, char* resource_id, JSON_Value *j_properties_obj) 
{
	JSON_Value *json_resource = json_value_init_object();

	__build_resource_item_common(json_resource, dev_id, resource_id);
	
	jsonval_dotset_value(json_resource, "representation", json_value_deep_copy(j_properties_obj));
	
    jsonarr_append_value(json_value_get_array(body->root), json_resource);
	return 0;
}


static void __build_sys_resource_item_common(JSON_Value *json_val, char* sys_id, char* resource_id)
{
	char buff[128];

	sprintf(buff, "/systems/%s/subscriptions/subscribeAll", sys_id);
	jsonval_dotset_string(json_val, "subscriptionReference", buff);

	sprintf(buff, "/systems/%s/resource/%s", sys_id, resource_id);
	jsonval_dotset_string(json_val, "eventReference", buff);
	
	jsonval_dotset_string(json_val, "eventType", "UPDATED");

    im_util_strftime_now(buff, "%Y-%m-%dT%H:%M:%S.");
    sprintf(buff+strlen(buff), "%03d", im_util_gettime_now_msec());
	jsonval_dotset_string(json_val, "eventTime", buff);

	return;
}


int im_body411sys_req_append_resource_representation(IMPacketBodyPtr body, char* sys_id, char* resource_id, JSON_Value *j_properties_obj) 
{
	JSON_Value *json_resource = json_value_init_object();

	__build_sys_resource_item_common(json_resource, sys_id, resource_id);
	
	jsonval_dotset_value(json_resource, "representation", json_value_deep_copy(j_properties_obj));
	
    jsonarr_append_value(json_value_get_array(body->root), json_resource);
	return 0;
}


