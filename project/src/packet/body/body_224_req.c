/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *  
 *  @brief IoTMakers Device packet; 디바이스 인증 요청
 *
 *  @section 샘플패킷

data - serialized_string:
{
    "devId": "XXXXX",
    "athnRqtNo": "XXXXX",
    "extrSysId": "OPEN_TCP_001PTL001_10XXXXXXXX"
}
 
 *
 */
#include "packet/body/body.h"

int im_body224_req_set_athnRqtNo(IMPacketBodyPtr body, char *dev_pw)
{
	jsonval_dotset_string(body->root, "deviceAuthKey", dev_pw);
    return 0;
}


int im_body224sys_req_set_athnRqtNo(IMPacketBodyPtr body, char *dev_pw)
{
	jsonval_dotset_string(body->root, "channelAuthKey", dev_pw);
    return 0;
}

