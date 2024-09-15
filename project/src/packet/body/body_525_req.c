/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *  
 *  @brief IoTMakers Device packet; 디바이스 제어데이터 요청
 *
 *  @section 샘플패킷

control data - serialized_string:

{
	"ectestObject": {
		"x": 10
	},
	"id": "testAllTypeUri"
}

 *
 */
#include "packet/body/body.h"

int im_body525_req_start_parse(IMPacketBodyPtr body) 
{
    return 0;
}

int im_body525_req_get_resourceId (IMPacketBodyPtr body, char **o_resourceId)
{
    if ( body->root == NULL ){
		return -1;
	}

	*o_resourceId = jsonval_dotget_string(body->root, "id");
	return 0;
}
