/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *  
 *  @brief IoTMakers Device packet; 디바이스 제어데이터 처리결과
 *
 *  @section 샘플패킷

report data - serialized_string:
{
    "extrSysId": "OPEN_TCP_001PTL001_10XXXXXXXX",
    "devCnvyDataVOs": [
        {
            "devId": "XXXXX",
            "cnvyRowVOs": [
                {
                    "occDt": "2017-08-24 11:03:48.226",
                    "snsnDataInfoVOs": [{
                        "dataTypeCd": "10001003",
                        "snsnVal": 0.7
                    }],
                    "strDataInfoVOs": [
                        {
                            "snsnTagCd": "result",
                            "strVal": "OK"
                        }
                    ]
                }
            ]
        }
    ]
}

 *
 */


#include "packet/body/body.h"

