#-*- coding:utf-8 -*-

import json

from IoTMakersDeviceClient import IoTMakersDeviceClient

imClient = IoTMakersDeviceClient();

##########################################
#   리소스 설정(제어) 요청 처리 핸들러
##########################################
def OnResourceSetRequestHandler(pktBody, dev_id, resource_id, properties_in_jstr): 
    print ("OnResourceSetRequestHandler()-->", dev_id, resource_id, properties_in_jstr)

    # YOUR CONTROL CODE HERE
    properties = json.loads(properties_in_jstr);

    print ( json.dumps(properties, indent=4));

    #IM_RESP_CODE_2004_Changed
    return 2004;


##########################################
#   특정 리소스(resource_id) 조회 요청 처리 핸들러
##########################################
def OnResourceRetrieveOneRequestHandler(pktBody, dev_id, resource_id): 
    print ("OnResourceSetRequestHandler()-->", dev_id, resource_id)

    # YOUR CONTROL CODE HERE
    properties = { 
            "ectestString": "hello",
            "ectestDouble": 99.55,
            "ectestInteger": 100,
            "ectestBoolean": True
        };

    rc = imClient.ImResourceRetrieveSetResource(pktBody, resource_id, json.dumps(properties))
    if rc < 0 :
        print("fail ImResourceRetrieveSetResource()");
        ## Internal Error
        return 5000;
    
    #IM_RESP_CODE_2000_OK
    return 2000;


##########################################
#   본 디바이스(dev_id)의 모든 리소스 조회 요청 처리 핸들러
##########################################
def OnResourceRetrieveAllRequestHandler(pktBody, dev_id): 
    print ("OnResourceSetRequestHandler()-->", dev_id)

    # YOUR CONTROL CODE HERE

    # 1. append FIRST RESOURCE
    resource_id = "testAllTypeUri";

    properties = { 
            "ectestString": "hello",
            "ectestDouble": 99.55
        };

    rc = imClient.ImResourceRetrieveAppendResource(pktBody, resource_id, json.dumps(properties))
    if rc < 0 :
        print("fail ImResourceRetrieveAppendResource()");
        ## Internal Error
        return 5000;


    # 2. append SECOND RESOURCE
    resource_id = "testAllTypeUri";

    properties = { 
            "ectestInteger": 100,
            "ectestBoolean": True
        };

    rc = imClient.ImResourceRetrieveAppendResource(pktBody, resource_id, json.dumps(properties))
    if rc < 0 :
        print("fail ImResourceRetrieveAppendResource()");
        ## Internal Error
        return 5000;

    #IM_RESP_CODE_2000_OK
    return 2000;



##########################################
#   리소스 값 전송 예제
##########################################
def test_resource_data():

  return '{  "create_time": "231110130226701",  "mission_code": "ktserving",  "mission_id": "00.00.00.E9.C2.43231110125351970",  "owner": "BR_D2023102001",  "robot_id": "",  "task": [    {      "seq": 0,      "status": "end",      "task_code": "delivering",      "task_data": {        "complete": 0,        "disinfection_mode": "",        "goal": [          "T-308"        ],        "item_list": [          {            "tray_id": "1"          }        ],        "map_id": "6988",        "repeat": 0,        "request": 0      },      "task_id": "00.00.00.E9.C2.432311101253519700"    },    {      "seq": 1,      "status": "end",      "task_code": "delivering",      "task_data": {        "complete": 0,        "disinfection_mode": "",        "goal": [          "T-0BUSSING"        ],        "item_list": [          {            "tray_id": "2"          }        ],        "map_id": "6988",        "repeat": 0,        "request": 0      },      "task_id": "00.00.00.E9.C2.432311101253519701"    },    {      "seq": 2,      "status": "arrival",      "task_code": "returning",      "task_data": {        "complete": 0,        "disinfection_mode": "",        "goal": [          "T-0START"        ],        "item_list": [],        "map_id": "6988",        "repeat": 0,        "request": 0      },      "task_id": "00.00.00.E9.C2.432311101253519702"    }  ]}';



def example_resource_send_notification(): 

    resource_id = "testAllTypeUri";

    properties = { 
            "ectestString": "hello",
            "ectestDouble": 99.55,
            "ectestInteger": 100,
            "ectestBoolean": True
        };

    #print(json.dumps(properties));
    
    # 1. 전송 패킷 준비
    rc = imClient.ImResourceNotificationInit()
    if rc < 0 :
        print("fail ImResourceNotificationInit()");
        return -1;


    # 2. 패킷에 리소스 추가; append FIRST RESOURCE
    #rc = imClient.ImResourceNotificationAppendResource(resource_id, json.dumps(properties));
    #print(test_resource_data());
    rc = imClient.ImResourceNotificationAppendResource(resource_id, test_resource_data());
    if rc < 0 :
        print("fail ImResourceNotificationAppendResource()");
        return -1;


    # 3. 패킷 전송
    rc = imClient.ImResourceNotificationSend();
    if rc < 0 :
        print("fail ImResourceNotificationSend()");
        return -1;

    
    print("OK, SENT: ", json.dumps(properties));


    return 0;





##########################################
# 시작 
##########################################
def iot_sample_run():

    '''
    IM_SERVER_ADDR = "dev-gw-iotmakers.cloud.kt.com";
    IM_SERVER_PORT = 30105;
    IM_DEV_ID = "__rbt_ssl_test__";
    IM_DEV_PW = "1234";
    IM_DEV_GW = "ROBOT_SSL_STD";
    IM_LOGLEVEL = 3;    # 1:ERR, 2:INFO, 3:DEBUG
    '''

    IM_SERVER_ADDR = "gw-iotmakers.cloud.kt.com";
    IM_SERVER_PORT = 30140;
    IM_DEV_ID = "TEST.NETWORK";
    IM_DEV_PW = "1234";
    IM_DEV_GW = "M_ROBOT_BEARROBOTICS";
    IM_LOGLEVEL = 2;    # 1:ERR, 2:INFO, 3:DEBUG


    print("ImInit()...");
    rc = imClient.ImInit(IM_LOGLEVEL);
    if rc < 0 :
        return;

    print("ImSetControlCallBackHandler()...");
    imClient.ImSetControlCallBackHandler(OnResourceSetRequestHandler, OnResourceRetrieveOneRequestHandler, OnResourceRetrieveAllRequestHandler);

    #print("ImSetTlsCaFile()...");
    #imClient.ImSetTlsCaFile("robotmakers.cer.pem");

    #imClient.ImTurnResponseWaitOff();

    while True:

        print("ImConnectTo()...", IM_SERVER_ADDR, IM_SERVER_PORT);
        rc = imClient.ImConnectTo(IM_SERVER_ADDR, IM_SERVER_PORT);
        if rc < 0 :
            imClient.ImRelease();
            return;


        print("ImAuthDevice()...", IM_DEV_ID, IM_DEV_PW, IM_DEV_GW);
        rc = imClient.ImAuthDevice(IM_DEV_ID, IM_DEV_PW, IM_DEV_GW);
        if rc < 0 :
            imClient.ImDisconnect();
            imClient.ImRelease();
            return;

        while True:
            rc = imClient.ImPoll();
            if rc < 0 :
                break;

            # 리소스 전송 예제
            
            rc = example_resource_send_notification();
            #if rc < 0 :
            #    break;
            
            imClient.ImMSleep(5000);


        print("ImDisconnect()...");
        rc = imClient.ImDisconnect();
        imClient.ImMSleep(1000);


    print("ImRelease()...");
    rc = imClient.ImRelease();


if __name__ == '__main__':
    while True:
        iot_sample_run()
        imClient.ImMSleep(1000);
