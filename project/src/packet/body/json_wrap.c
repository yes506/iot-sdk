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



#include <stdio.h>
#include <string.h>

#include "packet/body/json_wrap.h"

/** 
 *   @brief  jsonobj_dotset_value  
       JSON_Object의 dotpath위치에 JSON_Value 저장
 *  
 *   @param  jsobj is a JSON_Object
 *   @return int
 */  
int jsonobj_dotset_value(JSON_Object *jsobj, char *dotpath, JSON_Value *value) 
{
    if ( jsobj == NULL )    {
        return -1;
    }
    json_object_dotset_value(jsobj, dotpath, value);
    return 0;
}

 /** 
  *   @brief  jsonobj_dotget_value  
        JSON_Object의 dotpath위치에서 JSON_Value 참조
  *  
  *   @param  jsobj is a JSON_Object
  *   @return JSON_Value*
  */  
JSON_Value* jsonobj_dotget_value(JSON_Object *jsobj, char *dotpath)
{
    if ( jsobj == NULL )    {
        return NULL;
    }

    JSON_Value *jsonval = json_object_dotget_value(jsobj, dotpath);
    if ( jsonval == NULL )  {
        return NULL;
    }
    return jsonval;
}


 /** 
  *   @brief  jsonarr_append_value  
        JSON_Array 에 JSON_Value 저장
  *  
  *   @param  jsarr is a JSON_Array
  *   @return int
  */  
int jsonarr_append_value(JSON_Array *jsarr, JSON_Value *value) 
{
    if ( jsarr == NULL )    {
        return -1;
    }
    json_array_append_value(jsarr, value);
    return 0;
}

 /** 
  *   @brief  jsonarr_get_value_with_index  
        JSON_Object의 dotpath위치에서 JSON_Value 참조
  *  
  *   @param  jsarr is a JSON_Array
  *   @return JSON_Value*
  */  
JSON_Value* jsonarr_get_value_with_index(JSON_Array *jsarr, int idx)
{
    if ( jsarr == NULL )    {
        return NULL;
    }

    JSON_Value *jsonval = json_array_get_value(jsarr, idx);
    if ( jsonval == NULL )  {
        return NULL;
    }
    return jsonval;
}


 /** 
  *   @brief  jsonobj_dotget_array  
        JSON_Object의 dotpath위치에서 JSON_Array 참조
  *  
  *   @param  jsobj is a JSonStubPtr
  *   @return JSON_Array*
  */  
JSON_Array* jsonobj_dotget_array(JSON_Object *jsobj, char *dotpath)
{
    if ( jsobj == NULL )    {
        return NULL;
    }

    JSON_Value *pjval = jsonobj_dotget_value(jsobj, dotpath);
    if ( pjval == NULL )  {
        return NULL;
    }

    if ( json_type(pjval) != JSONArray )    {
        return NULL;
    }

    JSON_Array* pjarr = json_array  (pjval);
    return pjarr;
}

 /** 
  *   @brief  jsonarr_count_of_object  
        JSON_Array의 object수 참조
  *  
  *   @param  jsarr is a JSON_Array
  *   @return void
  */  
int jsonarr_count_of_object(JSON_Array * jsarr) 
{
    if ( jsarr == NULL )    {
        return -1;
    }
	return json_array_get_count(jsarr);
}


/* =================================== */

 /** 
  *   @brief  jsonobj_dotset_string  
        JSON_Object의 dotpath위치에 json_string을 객체화저장
  *  
  *   @param  jsobj is a JSON_Object
  *   @return int
  */  
int jsonobj_dotset_json_string(JSON_Object *jsobj, char *dotpath, char *json_string) 
{
    JSON_Value *jsonval = json_parse_string (json_string); 
    if ( jsonval == NULL )    {
        return -1;
    }
    return jsonobj_dotset_value(jsobj, dotpath, jsonval);
}



 /** 
  *   @brief  jsonobj_dotset_string  
        JSON_Object의 dotpath위치에 string 저장
  *  
  *   @param  jstub is a JSonStubPtr
  *   @return int
  */  
int jsonobj_dotset_string(JSON_Object *jsobj, char *dotpath, char *string) 
{
    JSON_Value *jsonval = json_value_init_string (string); 
    return jsonobj_dotset_value(jsobj, dotpath, jsonval);
}

 /** 
  *   @brief  jsonobj_dotset_number
        JSON_Object의 dotpath위치에 number 저장
  *  
  *   @param  jsobj is a JSON_Object
  *   @return int
  */  
int jsonobj_dotset_number(JSON_Object *jsobj, char *dotpath, double number) 
{
    JSON_Value *jsonval = json_value_init_number (number); 
    return jsonobj_dotset_value(jsobj, dotpath, jsonval);
}


 /** 
  *   @brief  jsonobj_dotset_bool
        JSON_Object의 dotpath위치에 bool 저장
  *  
  *   @param  jsobj is a JSON_Object
  *   @return int
  */  
int jsonobj_dotset_bool(JSON_Object *jsobj, char *dotpath, int boolVal) 
{
    JSON_Value *jsonval = json_value_init_boolean (boolVal); 
    return jsonobj_dotset_value(jsobj, dotpath, jsonval);
}


 /** 
  *   @brief  jsonobj_dotset_array
        JSON_Object의 dotpath위치에 array 생성
  *  
  *   @param  jsobj is a JSON_Object
  *   @return int
  */  
int jsonobj_dotset_array(JSON_Object *jsobj, char *dotpath) 
{
    JSON_Value *jsonval = json_value_init_array (); 
    return jsonobj_dotset_value(jsobj, dotpath, jsonval);
}


/*
IF json_object_get_array is NULL THEN json_value_init_array
*/
JSON_Array* jsonobj_check_array(JSON_Object *jsobj, char* dotpath)
{
	JSON_Array* arr = jsonobj_dotget_array (jsobj, dotpath);
	if ( arr == NULL )	{
		jsonobj_dotset_array(jsobj, dotpath);
        arr = jsonobj_dotget_array (jsobj, dotpath);
	}
	return arr;
}



/* =================================== */

 /** 
  *   @brief  jsonarr_append_string  
        JSON_Array에 json_string을 객체화 추가
  *  
  *   @param  jsarr is a JSON_Array
  *   @return int
  */  
int jsonarr_append_json_string(JSON_Array *jsarr, char *json_string) 
{
    JSON_Value *jsonval = json_parse_string (json_string); 
    if ( jsonval == NULL )    {
        return -1;
    }
    return jsonarr_append_value(jsarr, jsonval);
}


 /** 
  *   @brief  jsonarr_append_string  
        JSON_Array에 string 추가
  *  
  *   @param  jsarr is a JSON_Array
  *   @return int
  */  
int jsonarr_append_string(JSON_Array *jsarr, char *string) 
{
    JSON_Value *jsonval = json_value_init_string (string); 
    return jsonarr_append_value(jsarr, jsonval);
}

 /** 
  *   @brief  jsonarr_append_number
        JSON_Array에 number 추가
  *  
  *   @param  jsarr is a JSON_Array
  *   @return int
  */  
int jsonarr_append_number(JSON_Array *jsarr, double number) 
{
    JSON_Value *jsonval = json_value_init_number (number); 
    return jsonarr_append_value(jsarr, jsonval);
}

 /** 
  *   @brief  jsonarr_append_array
        JSON_Array에 새 array 추가
  *  
  *   @param  jsarr is a JSON_Array
  *   @return int
  */  
int jsonarr_append_array(JSON_Array *jsarr) 
{
    JSON_Value *jsonval = json_value_init_array (); 
    return jsonarr_append_value(jsarr, jsonval);
}

/* =================================== */


 /** 
  *   @brief  jsonobj_dotget_string  
        JSON_Object의 dotpath위치에서 string 참조
  *  
  *   @param  jsobj is a JSON_Object
  *   @return NULL on fail
  */  
char* jsonobj_dotget_string(JSON_Object *jsobj, char *dotpath) 
{
    JSON_Value  *pjval =  jsonobj_dotget_value(jsobj, dotpath);
	return (char*)json_string(pjval);
}

/** 
  *   @brief  jsonobj_dotget_number  
        JSON_Object의 dotpath위치에서 number 참조
  *  
  *   @param  jsobj is a JSON_Object
  *   @return 0 on fail
  */  
double jsonobj_dotget_number(JSON_Object *jsobj, char *dotpath) 
{
    JSON_Value  *pjval =  jsonobj_dotget_value(jsobj, dotpath);
	return (double)json_number(pjval);
}

int jsonobj_dotget_bool(JSON_Object *jsobj, char *dotpath) 
{
    JSON_Value  *pjval =  jsonobj_dotget_value(jsobj, dotpath);
	return json_boolean(pjval);
}


JSON_Value* jsonval_dotget_value(JSON_Value *jsval, char *dotpath)
{
    return jsonobj_dotget_value(json_object(jsval), dotpath);
}
char* jsonval_dotget_string(JSON_Value *jsval, char *dotpath)
{
    return jsonobj_dotget_string(json_object(jsval), dotpath);
}
double jsonval_dotget_number(JSON_Value *jsval, char *dotpath)
{
    return jsonobj_dotget_number(json_object(jsval), dotpath);
}


int jsonval_dotget_bool(JSON_Value *jsval, char *dotpath)
{
    return jsonobj_dotget_bool(json_object(jsval), dotpath);
}

int jsonval_dotset_string(JSON_Value *jsval, char *dotpath, char *string)
{
    return jsonobj_dotset_string(json_object(jsval), dotpath, string);
}
int jsonval_dotset_number(JSON_Value *jsval, char *dotpath, double number)
{
    return jsonobj_dotset_number(json_object(jsval), dotpath, number);
}
int jsonval_dotset_bool(JSON_Value *jsval, char *dotpath, int boolVal)
{
    return jsonobj_dotset_bool(json_object(jsval), dotpath, boolVal);
}


int jsonval_dotset_value(JSON_Value *jsval, char *dotpath, JSON_Value *val)
{
    return jsonobj_dotset_value(json_object(jsval), dotpath, val);
}


JSON_Value* jsonval_dotget_value_on_arrray_with_index(JSON_Value* jsval, char *arrname, int idx)
{
    JSON_Array *arr = jsonobj_dotget_array(json_object(jsval), arrname);
    if ( arr == NULL )    {
        return (JSON_Value*)NULL;
    }
    return jsonarr_get_value_with_index(arr, idx);
}


int jsonval_append_value_to_arrray(JSON_Value* jsval, char* arrname, JSON_Value* val)
{
    JSON_Array *arr = jsonobj_check_array(json_object(jsval), arrname);
    if ( arr == NULL )    {
        return -1;
    }
    return jsonarr_append_value(arr, val);
}


/** 
  *   @brief  jsonarr_get_string_with_index  
        JSON_Array 의 idx 위치에서 string 참조
  *  
  *   @param  jsarr is a JSON_Array
  *   @return NULL on fail
  */  
char* jsonarr_get_string_with_index(JSON_Array *jsarr, int idx) 
{
    JSON_Value  *pjval =  jsonarr_get_value_with_index(jsarr, idx);
	return (char*)json_string(pjval);
}

/** 
  *   @brief  jsonarr_get_number_with_index  
        JSON_Array 의 idx 위치에서 number 참조
  *  
  *   @param  jsarr is a JSON_Array
  *   @return NULL on fail
  */  
double jsonarr_get_number_with_index(JSON_Array *jsarr, int idx) 
{
    JSON_Value  *pjval =  jsonarr_get_value_with_index(jsarr, idx);
	return (double)json_number(pjval);
}



char* jsonval_to_string(JSON_Value* jVal)
{
	return (char*)json_string(jVal);
}

double jsonval_to_number(JSON_Value* jVal)
{
    return json_number(jVal);
}

