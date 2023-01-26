#include "requests.h"
#include "cJSON_lib/cJSON.h"
#include "http_parser/picohttpparser.h"
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

request *get_request(int socket)
{
    request *result = malloc(sizeof(request));

    if (!result)
    {
        fprintf(stderr, "Memory allocation failed!\n");
        return NULL;
    }

    int buf_size = 1024;
    char *buf = malloc(buf_size * sizeof(char));
    if (!buf)
    {
        fprintf(stderr, "Memory allocation failed!\n");
        free(result);
        return NULL;
    }
    memset(buf, '\0', buf_size * sizeof(char));
    struct phr_header headers[100];
    size_t num_headers = sizeof(headers) / sizeof(headers[0]);
    size_t buf_length = 0;
    ssize_t bytes;
    int parsed;
    while ((bytes = read(socket, buf + buf_length, buf_size - buf_length)) == -1 && errno == EINTR)
        ;
    if (bytes <= 0)
    {
        printf("Problem with connection");
        free(buf);
        free(result);
        return NULL;
    }
    buf_length += bytes;
    size_t method_len, path_len;
    const char *path, *method;
    int minor_version;
    parsed = phr_parse_request(buf, buf_length, &method, &method_len, &path, &path_len, &minor_version, headers,
                               &num_headers, 0);
    if (parsed == -1)
    {
        printf("Problem with parsing");
        free(buf);
        free(result);
        return NULL;
    }
    int content_length = 0;
    for (int i = 0; i < num_headers; i++)
    {
        if (strncmp("Content-Length", headers[i].name, headers[i].name_len) == 0)
        {
            content_length = atoi(headers[i].value);
            char *end_request_ptr = strstr(buf, "\r\n\r\n");
            if (end_request_ptr != NULL &&
                buf_size - ((end_request_ptr + 4 - buf) / sizeof(char)) >= content_length + 1)
            {
                break;
            }
            buf_size += content_length + 1;
            buf = realloc(buf, buf_size);
            memset(buf + buf_length, '\0', (content_length + 1) * sizeof(char));
            break;
        }
    }
    bool should_read = true;
    char *end_request_ptr = strstr(buf, "\r\n\r\n");
    if (end_request_ptr != NULL && strlen(end_request_ptr + 4) == content_length)
    {
        should_read = false;
    }
    while (should_read)
    {
        while ((bytes = read(socket, buf + buf_length, buf_size - buf_length)) == -1 && errno == EINTR)
            ;
        if (bytes <= 0)
        {
            printf("Problem with connection");
            free(buf);
            free(result);
            return NULL;
        }
        buf_length += bytes;
        parsed = phr_parse_request(buf, buf_length, &method, &method_len, &path, &path_len, &minor_version, headers,
                                   &num_headers, 0);
        if (parsed == -1)
        {
            printf("Problem with parsing");
            free(buf);
            free(result);
            return NULL;
        }
        end_request_ptr = strstr(buf, "\r\n\r\n");
        if (end_request_ptr != NULL && strlen(end_request_ptr + 4) == content_length)
        {
            should_read = false;
        }
    }

    result->path = malloc(path_len + 1);
    strncpy(result->path, path, path_len);
    result->path[path_len] = '\0';
    result->method = malloc(method_len + 1);
    strncpy(result->method, method, method_len);
    result->method[method_len] = '\0';
   
    result->has_data = false;
    char *data_ptr = strstr(buf, "\r\n\r\n");
    if (data_ptr != NULL)
    {
        char *body_ptr = data_ptr + 4;
        if (strlen(body_ptr) > 0)
        {
            result->data = malloc(strlen(body_ptr) + 1);
            strncpy(result->data, body_ptr, strlen(body_ptr));
            result->data[strlen(body_ptr)] = '\0';
            result->has_data = true;
        }
    }
    free(buf);
    return result;
}

void destroy_request(request *request)
{
    free(request->path);
    free(request->method);
    
    if (request->has_data == true)
    {
        free(request->data);
    }
    free(request);
}


char *get_response(http_status status, cJSON *payload)
{
    char *response;
    char *body = malloc(sizeof(char));
    body[0] = '\0';
    if (payload != NULL)
    {
        free(body);
        body = cJSON_Print(payload);
        response = malloc(sizeof(char) * (300 + strlen(body)));
    }
    else
    {
        response = malloc(sizeof(char) * 300);
    }
    char *status_str;
    if (status == HTTP_OK)
    {
        status_str = "200 OK";
    }
    else if (status == HTTP_BAD_REQUEST)
    {
        status_str = "400 Bad Request";
    }
    else if (status == HTTP_DISALLOWED_METHOD)
    {
        status_str = "405 Method Not Allowed";
    }
    else if (status == HTTP_UNAUTHORIZED)
    {
        status_str = "401 Unauthorized";
    }
    else if (status == HTTP_NOT_FOUND)
    {
        status_str = "404 Not Found";
    }
    else
    {
        status_str = "500 Internal Server Error";
    }
    sprintf(response,
            "HTTP/1.1 %s\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Headers: "
            "*\r\nAccess-Control-Allow-Methods: *\r\nContent-Type: application/json\r\n\r\n%s",
            status_str, body);
    free(body);
    return response;
}
