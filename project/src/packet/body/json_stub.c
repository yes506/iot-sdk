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

#include "packet/body/json_stub.h"


void json_tool_test()
{
    printf("%s\n", "sss");
}
void jsonstub_print(JSonStubPtr jstub)
{
    char *serialized_string = NULL;
	serialized_string = json_serialize_to_string_pretty(jstub->root_val);
	printf("serialized_jstub:\n%s\n", serialized_string);
    json_free_serialized_string(serialized_string);
}


 /** 
  *   @brief  jsonstub_init  
  *  
  *   @param  jstub is a JSonStubPtr
  *   @return void
  */  
int jsonstub_init(JSonStubPtr jstub) 
{
	jstub->json_status = JSONSuccess;
	jstub->serialized_string = NULL;
	jstub->root_val = NULL;
	return 0;
}

 /** 
  *   @brief  jsonstub_release  
  *  
  *   @param  jstub is a JSonStubPtr
  *   @return void
  */  
void jsonstub_release(JSonStubPtr jstub) 
{
	if ( jstub->serialized_string != NULL ){
		json_free_serialized_string(jstub->serialized_string);
		jstub->serialized_string = NULL;
	}

	if ( jstub->root_val != NULL ){
		json_value_free(jstub->root_val);
		jstub->root_val = NULL;
	}
}

 /** 
  *   @brief  jsonstub_init_as_object  
  *  
  *   @param  jstub is a JSonStubPtr
  *   @return void
  */  
int jsonstub_init_as_object(JSonStubPtr jstub) 
{
    jsonstub_init(jstub);

    jstub->root_val = json_value_init_object();
	if ( jstub->root_val == NULL )	{
		return -1;
	}
	return 0;
}

 /** 
  *   @brief  jsonstub_init_as_array  
  *  
  *   @param  jstub is a JSonStubPtr
  *   @return void
  */  
int jsonstub_init_as_array(JSonStubPtr jstub) 
{
    jsonstub_init(jstub);

    jstub->root_val = json_value_init_array();
	if ( jstub->root_val == NULL )	{
		return -1;
	}
	return 0;
}

 /** 
  *   @brief  jsonstub_init_init_with_json_string  
  *  
  *   @param  jstub is a JSonStubPtr
  *   @return void
  */  
int jsonstub_init_with_json_string(JSonStubPtr jstub, char *json_str) 
{
    jsonstub_init(jstub);

    jstub->root_val = json_parse_string(json_str);
	if ( jstub->root_val == NULL )	{
		return -1;
	}
	return 0;
}


/* =================================== */

 /** 
  *   @brief  jsonstub_dotset_value  
  *  
  *   @param  jstub is a JSonStubPtr
  *   @return void
  */  
int jsonstub_dotset_value(JSonStubPtr jstub, char *dotpath, JSON_Value *value) 
{
	if ( jstub->root_val == NULL )	{
		return -1;
	}
    jstub->json_status = json_object_dotset_value(json_value_get_object(jstub->root_val), dotpath, value);
    if ( jstub->json_status != 0 )  {
        return -1;
    }
    return 0;
}

 /** 
  *   @brief  jsonstub_dotget_value  
  *  
  *   @param  jstub is a JSonStubPtr
  *   @return void
  */  
JSON_Value* jsonstub_dotget_value(JSonStubPtr jstub, char *dotpath)
{
	if ( jstub->root_val == NULL )	{
		return NULL;
	}

    JSON_Value *jsonval = json_object_dotget_value(json_value_get_object(jstub->root_val), dotpath);
    if ( jsonval == NULL )  {
        return NULL;
    }
    return jsonval;
}

 /** 
  *   @brief  jsonstub_dotget_array  
  *  
  *   @param  jstub is a JSonStubPtr
  *   @return void
  */  
JSON_Array* jsonstub_dotget_array(JSonStubPtr jstub, char *dotpath)
{
    JSON_Value *pjval = NULL;

    if ( jstub->root_val == NULL )	{
        return NULL;
	}

    if ( dotpath == NULL  )    {
        pjval = jstub->root_val;
    } else {
        pjval = json_object_dotget_value(json_value_get_object(jstub->root_val), dotpath);
    }

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
  *   @brief  jsonstub_dotset_value  
  *  
  *   @param  jstub is a JSonStubPtr
  *   @return void
  */  
int jsonstub_dotget_count_of_array(JSonStubPtr jstub, char *dotpath) 
{
	JSON_Array* pjarr = jsonstub_dotget_array  (jstub, dotpath);
    if ( pjarr == NULL )    {
        return -1;
    }
	return json_array_get_count(pjarr);
}


/*
IF json_object_get_array is NULL THEN json_value_init_array
*/
JSON_Array* jsonstub_dotset_array(JSonStubPtr jstub, char *dotpath, char *arrname)
{
    JSON_Value *jsonval = jsonstub_dotget_value(jstub, dotpath);

	JSON_Array* pjarr = json_object_get_array  (json_value_get_object(jsonval), arrname);
	if ( pjarr == NULL )	{
		json_object_set_value(json_value_get_object(jsonval), arrname, json_value_init_array());
	}
    return json_object_get_array  (json_value_get_object(jsonval), arrname);
}

/* =================================== */


 /** 
  *   @brief  jsonstub_dotset_string  
  *  
  *   @param  jstub is a JSonStubPtr
  *   @return void
  */  
int jsonstub_dotset_string(JSonStubPtr jstub, char *dotpath, char *string) 
{
    JSON_Value *jsonval = json_value_init_string (string); 
    return jsonstub_dotset_value(jstub, dotpath, jsonval);
}

int jsonstub_dotset_string_appent_to_array(JSonStubPtr jstub, char *dotpath, char *string) 
{
    JSON_Value *jsonval = json_value_init_string (string); 
    return jsonstub_dotset_value(jstub, dotpath, jsonval);
}

 /** 
  *   @brief  jsonstub_dotset_number  
  *  
  *   @param  jstub is a JSonStubPtr
  *   @return void
  */  
int jsonstub_dotset_number(JSonStubPtr jstub, char *dotpath, double number) 
{
    JSON_Value *jsonval = json_value_init_number (number); 
    return jsonstub_dotset_value(jstub, dotpath, jsonval);
}

int jsonstub_dotset_number_appent_to_array(JSonStubPtr jstub, char *dotpath, char *string) 
{
    JSON_Value *jsonval = json_value_init_string (string); 
    return jsonstub_dotset_value(jstub, dotpath, jsonval);
}


 /** 
  *   @brief  jsonstub_dotget_string  
  *  
  *   @param  jstub is a JSonStubPtr
  *   @return NULL on fail
  */  
char* jsonstub_dotget_string(JSonStubPtr jstub, char *dotpath) 
{
    JSON_Value  *pjval =  jsonstub_dotget_value(jstub, dotpath);
	return (char*)json_string(pjval);
}
char* jsonstub_dotget_string_from_array_with_index(JSonStubPtr jstub, char *dotpath, int idx) 
{
	JSON_Array* pjarr = jsonstub_dotget_array  (jstub, dotpath);
	return (char*)json_array_get_string(pjarr, idx);
}

 /** 
  *   @brief  jsonstub_dotget_number  
  *  
  *   @param  jstub is a JSonStubPtr
  *   @return 0 on fail
  */  
double jsonstub_dotget_number(JSonStubPtr jstub, char *dotpath) 
{
    JSON_Value  *pjval =  jsonstub_dotget_value(jstub, dotpath);
	return (double)json_number(pjval);
}

double jsonstub_dotget_number_from_array_with_index(JSonStubPtr jstub, char *dotpath, int idx) 
{
	JSON_Array* pjarr = jsonstub_dotget_array  (jstub, dotpath);
	return (double)json_array_get_number(pjarr, idx);
}



