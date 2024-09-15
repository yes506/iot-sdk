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



#ifndef JSON_PARSON_TOOL_H
#define JSON_PARSON_TOOL_H

#include "parson/parson.h"

typedef struct {
    JSON_Value		*root_val;
    char			*serialized_string;
	JSON_Status		json_status;
} JSonStub, *JSonStubPtr;


#ifdef __cplusplus
extern "C"
{
#endif


void json_tool_test();
void jsonstub_print(JSonStubPtr jstub);
int jsonstub_init(JSonStubPtr jstub) ;
void jsonstub_release(JSonStubPtr jstub) ;
int jsonstub_init_as_object(JSonStubPtr jstub) ;
int jsonstub_init_as_array(JSonStubPtr jstub) ;
int jsonstub_init_with_json_string(JSonStubPtr jstub, char *json_str) ;
int jsonstub_dotset_value(JSonStubPtr jstub, char *dotpath, JSON_Value *value) ;
JSON_Value* jsonstub_dotget_value(JSonStubPtr jstub, char *dotpath);
JSON_Array* jsonstub_dotget_array(JSonStubPtr jstub, char *dotpath);
int jsonstub_dotget_count_of_array(JSonStubPtr jstub, char *dotpath) ;
JSON_Array* jsonstub_dotset_array(JSonStubPtr jstub, char *dotpath, char *arrname);
int jsonstub_dotset_string(JSonStubPtr jstub, char *dotpath, char *string) ;
int jsonstub_dotset_string_appent_to_array(JSonStubPtr jstub, char *dotpath, char *string) ;
int jsonstub_dotset_number(JSonStubPtr jstub, char *dotpath, double number) ;
int jsonstub_dotset_number_appent_to_array(JSonStubPtr jstub, char *dotpath, char *string) ;
char* jsonstub_dotget_string(JSonStubPtr jstub, char *dotpath) ;
char* jsonstub_dotget_string_from_array_with_index(JSonStubPtr jstub, char *dotpath, int idx) ;
double jsonstub_dotget_number(JSonStubPtr jstub, char *dotpath) ;
double jsonstub_dotget_number_from_array_with_index(JSonStubPtr jstub, char *dotpath, int idx) ;


#ifdef __cplusplus
}
#endif



#endif



