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



#ifndef JSON_PARSON_TOOL_WRAPPER_H
#define JSON_PARSON_TOOL_WRAPPER_H

#include "parson/parson.h"


#ifdef __cplusplus
extern "C"
{
#endif


JSON_Value* jsonobj_dotget_value(JSON_Object *jsobj, char *dotpath);
JSON_Array* jsonobj_dotget_array(JSON_Object *jsobj, char *dotpath);
char*       jsonobj_dotget_string(JSON_Object *jsobj, char *dotpath) ;
double      jsonobj_dotget_number(JSON_Object *jsobj, char *dotpath) ;

int         jsonobj_dotset_json_string(JSON_Object *jsobj, char *dotpath, char *json_string) ;
int         jsonobj_dotset_value(JSON_Object *jsobj, char *dotpath, JSON_Value *value) ;
int         jsonobj_dotset_array(JSON_Object *jsobj, char *dotpath) ;
int         jsonobj_dotset_string(JSON_Object *jsobj, char *dotpath, char *string) ;
int         jsonobj_dotset_number(JSON_Object *jsobj, char *dotpath, double number) ;

JSON_Array* jsonobj_check_array(JSON_Object *jsobj, char* dotpath);

int         jsonarr_append_value(JSON_Array *jsarr, JSON_Value *value) ;
int         jsonarr_append_json_string(JSON_Array *jsarr, char *json_string) ;
int         jsonarr_append_string(JSON_Array *jsarr, char *string) ;
int         jsonarr_append_number(JSON_Array *jsarr, double number) ;
int         jsonarr_append_array(JSON_Array *jsarr) ;

int         jsonarr_count_of_object(JSON_Array * jsarr) ;
JSON_Value* jsonarr_get_value_with_index(JSON_Array *jsarr, int idx);
char*       jsonarr_get_string_with_index(JSON_Array *jsarr, int idx) ;
double      jsonarr_get_number_with_index(JSON_Array *jsarr, int idx) ;


JSON_Value* jsonval_dotget_value(JSON_Value *jsval, char *dotpath);
char*       jsonval_dotget_string(JSON_Value *jsval, char *dotpath) ;
double      jsonval_dotget_number(JSON_Value *jsval, char *dotpath) ;

JSON_Value* jsonval_dotget_value_on_arrray_with_index(JSON_Value *jsval, char* arrname, int idx);
int         jsonval_append_value_to_arrray(JSON_Value* jsval, char* arrname, JSON_Value* val);


int         jsonval_dotset_string(JSON_Value *jsval, char *dotpath, char *string) ;
int         jsonval_dotset_number(JSON_Value *jsval, char *dotpath, double number) ;
int         jsonval_dotset_value(JSON_Value *jsval, char *dotpath, JSON_Value *val) ;



char*  jsonval_to_string(JSON_Value* jVal);
double jsonval_to_number(JSON_Value* jVal);



int jsonval_dotget_bool(JSON_Value *jsval, char *dotpath);
int jsonval_dotset_string(JSON_Value *jsval, char *dotpath, char *string);
int jsonval_dotset_number(JSON_Value *jsval, char *dotpath, double number);
int jsonval_dotset_bool(JSON_Value *jsval, char *dotpath, int boolVal);



#ifdef __cplusplus
}
#endif



#endif



