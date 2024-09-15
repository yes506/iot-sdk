/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *  
 *  @brief IoTMakers Device packet; 디바이스 최종값조회 요청
 *
 *  @section 샘플패킷

IF-711-req - serialized_string:
{
     "extrSysId": "HOME_IOT",
     "inclDevIds": ["W_085DDD27FB7507"],
     "excluDevIds": [],
     "cmdDataInfoVOs": [{
         "dataTypeCd": "2502",
         "cmdData": []
     }]
 } 
 



{
	"extrSysId": "DEV001",
	"inclDevIds": [
		"testbyw"
	],
	"groupTagCd": "testAllType",
	"msgHeadVO": {
		"mapHeaderExtension": {}
	}
}


 *
 */
#include "packet/body/body.h"

int im_body711_req_start_parse(IMPacketBodyPtr body) 
{
    // 데이터 핸들링을 위한 초기화
    return 0;
}
