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


#include <string.h>
#include <stdio.h>

#include "util/util.h"
#include "packet/body/body.h"



/* =====================================
debug
====================================== */
void bodyutil_JSON_Value_print(JSON_Value* jval)
{
    char *serialized_string = NULL;
	serialized_string = json_serialize_to_string_pretty(jval);
	printf("jval:\n%s\n", serialized_string);
    json_free_serialized_string(serialized_string);
}
