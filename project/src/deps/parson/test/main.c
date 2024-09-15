#include "../parson.h"
#include "../parson.c"


/***


gcc main.c -o tt

valgrind  --leak-check=full  ./tt


*/

static int pars(char *j_str)
{
  printf("%s\n", j_str);

  JSON_Value *j_val = json_parse_string_with_comments(j_str);
  //JSON_Value *j_val = json_parse_string(j_str);
  
  if ( j_val == NULL )  {
    printf("%s\n", "error");
    return -1;
  }

  JSON_Object *j_obj = json_object (j_val);
  if ( j_obj == NULL )  {
    printf("%s\n", "error");
    return -1;
  }

  size_t cnt = json_object_get_count(j_obj);
  int i = 0;
  for ( i = 0 ; i < cnt; i++ )  {
    const char *name = json_object_get_name(j_obj, i);
    printf("%d = %s\n", i, name);

    JSON_Value *j_val_i = json_object_dotget_value(j_obj, name);
    if ( j_val_i == NULL )  {
      printf("%s\n", "error");
      return -1;
    }

    JSON_Value_Type type = json_value_get_type (j_val_i);
    printf("type = %d\n", type);
    switch ( type )
    {
    case JSONString:
      printf("type = %s\n", "JSONString");
      const char *s_val = json_string(j_val_i);
      printf("val = [%s]\n", s_val);
      break;  
    case JSONNumber:
      printf("type = %s\n", "JSONNumber");
      double n_val = json_number(j_val_i);
      printf("val = [%f]\n", n_val);
      break;  
    case JSONObject:
    case JSONArray: {
      char *buff = json_serialize_to_string(j_val_i);
      printf("buff = %s\n", buff);
      json_free_serialized_string(buff);
    }
      break;  
    default:
      printf("not supported type = %d\n", type);

    }
  }


  json_value_free(j_val);

  return 0;
}


int main()
{
  char *j_str = "{\"num\":100, \"str\":\"hello\", \"str2\":{}   }";

  while (1)
  {
    pars(j_str);
  }


  return 0;
}

