#ifndef CHAT_ACTIONS_H
#define CHAT_ACTIONS_H

#include "cJSON_lib/cJSON.h"
#include "http_status.h"
#include <stdbool.h>


cJSON *get_users(cJSON *request, int user_id, http_status *response_status);
cJSON *add_message(cJSON *request);
cJSON *get_messages(cJSON *request, int user_id, http_status *response_status);
cJSON *add_user(cJSON *request, int user_id);

#endif
