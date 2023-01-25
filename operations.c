#include "operations.h"
#include "cJSON_lib/cJSON.h"
#include <string.h>
#include "chat_actions.h"
#include "http_status.h"
#include <stdbool.h>

typedef cJSON *(operation)(cJSON * request, int client_id, http_status * response status);

bool is_operation(request *request, char *method, char *path);
operation get_operation(request *request);
cJSON *get_options(cJSON *request, int client_id, http_status *response_status);
cJSON *disallowed_method(cJSON *request, int client_id, http_status *response_status);


char *handle_request(request *request)
{
    cJSON *request_json = NULL;
    if(request-> has_data)
    {
        request_json = cJSON_Parse(request->data);
    }

    operation operation = get_operation(request);
    http_status status;
    int client_id = get_client_id();
    cJSON *response_json = operation(request_json, client_id, &status);
    char *response = get_response(status, response_json);

    if (response_json != NULL)
    {
        cJSON_Delete(response_json);
    }

    if (request->has_data)
    {
        cJSON_Delete(request_json);
    }
    return response;
    
}

bool is_operation(request *request, char *method, char *path)
{
    if (strcmp(request->method, method) == 0 && strcmp(request->path, path) == 0)
    {
        return true;
    }
    return false;
}

operation get_operation(request *request)
{
    if(strcmp(request->method, "OPTIONS") == 0)
    {
        return &get_options;
    }
    else if (is_operation(request, "GET", "/users"))
    {
        return &get_users;
    }
    else if(is_operation(request, "GET", "/messages"))
    {
        return &get_messages;
    }
    return &disallowed_method;
}

cJSON *get_options(cJSON *request, int client_id, http_status *response_status)
{
    *response_status = HTTP_OK;
    return NULL;
}

cJSON *disallowed_method(cJSON *request, int client_id, http_status *response_status)
{
    *response_status = HTTP_DISALLOWED_METHOD;
    return NULL;
}
