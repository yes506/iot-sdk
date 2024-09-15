#include <stdio.h>

#define _LINUX

#include "http-client-c.h"

/*
struct http_response
{
	struct parsed_url *request_uri;
	char *body;
	char *status_code;
	int status_code_int;
	char *status_text;
	char *request_headers;
	char *response_headers;
};
*/


#define HTTP_NIC_NAME    "ens38"


static char* file_name = "data.bin";
static FILE *fp = NULL;
static int file_size = 0;

int  open_save_file()
{
    if ( (fp = fopen(file_name, "wb")) == NULL )    {
        printf("fail file open\n");
        return -1;
    }

    return 0;
}

void  close_save_file()
{
    fclose(fp);
}

void download_handler(char *body_part, int part_len)
{
    if ( part_len == 0)    {
        printf("End Of Data\n");
        return;
    }

    if ( fp != NULL )    {
        int w_len = fwrite( (void*)body_part, part_len, (1), fp );

        if ( w_len == (1) )        {
            file_size += part_len;
            printf("OK fwrite; size=[%d]\n", file_size);

        } else {
            printf("FAIL fwrite\n");
        }
    }
}


int test_http_get()
{
    char *url = "http://www.google.com";
    struct http_response* resp = http_get(url, NULL);

    if ( resp == NULL )    {
        printf("Failed http_get\n");
        return -1;
    }

    printf("status_code_int=[%d]\n", resp->status_code_int);
    /*
    printf("request_uri->ip=[%s]\n", resp->request_uri->ip);
    printf("request_uri->uri=[%s]\n", resp->request_uri->uri);
    
    printf("status_code=[%s]\n", resp->status_code);
    printf("status_text=[%s]\n", resp->status_text);
    printf("request_headers=[%s]\n", resp->request_headers);
    printf("response_headers=[%s]\n", resp->response_headers);
    */
    return 0;
}


int test_http_download_get()
{
    char *url = "http://i.ytimg.com/vi/O8IwxzfiO9g/maxresdefault.jpg";
    //char *url = "http://172.217.27.150:80/vi/O8IwxzfiO9g/maxresdefault.jpg";

    printf("fopen [%s]\n", file_name);
    if ( open_save_file() < 0 )    {
        printf("fail file open\n");
        return -1;
    }

    struct http_response* resp = http_get_with_callback(url, NULL, download_handler);
    printf("fclose [%s]\n", file_name);
    close_save_file();

    if ( resp == NULL )    {
        printf("Failed http_get\n");
        return -1;
    }

    printf("status_code_int=[%d]\n", resp->status_code_int);
    printf("request_uri->ip=[%s]\n", resp->request_uri->ip);
    printf("request_uri->uri=[%s]\n", resp->request_uri->uri);

    /*
    printf("status_code=[%s]\n", resp->status_code);
    printf("status_text=[%s]\n", resp->status_text);
    printf("request_headers=[%s]\n", resp->request_headers);
    printf("response_headers=[%s]\n", resp->response_headers);
    */
    return 0;
}



int test_http_download_post()
{
    /*
    OCF 5G 무선망
     http://10.10.10.109:2231/REWXNTK2NZYXNDEWMTKX 

    요청 시 Body (Json)
    {
        "extrSysId":"DM_TEST",
        "devId":"eMTC_DEV_0001",
        "devAthnRqtNo":"1234"
    }

    curl -i -X POST -H "Content-Type: application/json; charset=utf-8" -d '{"devId":"eMTC_DEV_0001","devAthnRqtNo":"1234"}' "http://10.10.10.109:2231/REWXNTK2NZYXNDEWMTKX"
    curl -i -X POST -H "Content-Type: application/json; charset=utf-8" -d '{"devId":"eMTC_DEV_0001","devAthnRqtNo":"1234"}' "http://127.0.0.1:2231/REWXNTK2NZYXNDEWMTKX"

    curl -i -X POST -H "Content-Type: application/json; charset=utf-8" -d '{"devId":"eMTC_DEV_0001","devAthnRqtNo":"1234"}' "http://211.252.123.36:10000/REWXNTK2NZKYOTQ0MJU5"


    PUT /REWXNTK2NZYXNDEWMTKX HTTP/1.1
    Host: 127.0.0.1:2231
    User-Agent: curl/7.47.0
    Accept: *\/*
    Content-Type: application/json; charset=utf-8
    Content-Length: 69

    "{\"devId\":\"eMTC_DEV_0001\",\"devAthnRqtNo\":\"1234\"}"
    */


    //char *url = "http://10.10.10.109:2231/REWXNTK2NZYXNDEWMTKX";
    //char *url = "http://127.0.0.1:2231/REWXNTK2NZYXNDEWMTKX";
    //char *url = "http://[fe80::20c:29ff:fe69:a422]:2231/REWXNTK2NZYXNDEWMTKX";


    //char *url = "http://i.ytimg.com/vi/O8IwxzfiO9g/maxresdefault.jpg";
    //char *url = "http://216.58.220.118:80/vi/O8IwxzfiO9g/maxresdefault.jpg";
    char *url = "http://[fe80::20c:29ff:fee8:13ab]:10004/vi/O8IwxzfiO9g/maxresdefault.jpg";


    printf("fopen [%s]\n", file_name);
    if ( open_save_file() < 0 )    {
        printf("fail file open\n");
        return -1;
    }

    //struct http_response* resp = http_post_json_with_callback(url, NULL, "{\"extrSysId\":\"DM_TEST\",\"devId\":\"eMTC_DEV_0001\",\"devAthnRqtNo\":\"1234\"}", download_handler);
    struct http_response* resp = http_post_json_with_callback(url, NULL, "{\"devId\":\"eMTC_DEV_0001\",\"devAthnRqtNo\":\"1234\"}", download_handler);


    printf("fclose [%s]\n", file_name);
    close_save_file();

    if ( resp == NULL )    {
        printf("Failed http_post_json_with_callback\n");
        return -1;
    }




    printf("status_code_int=[%d]\n", resp->status_code_int);
    printf("request_uri->ip=[%s]\n", resp->request_uri->ip);
    printf("request_uri->uri=[%s]\n", resp->request_uri->uri);

    /*
    printf("status_code=[%s]\n", resp->status_code);
    printf("status_text=[%s]\n", resp->status_text);
    printf("request_headers=[%s]\n", resp->request_headers);
    printf("response_headers=[%s]\n", resp->response_headers);
    */

    http_response_free(resp);

    return 0;
}





int main()
{
    //set_http_downloader_nic4ipv6(HTTP_NIC_NAME);

    test_http_download_get();
    //test_http_download_post();

    return 0;
}