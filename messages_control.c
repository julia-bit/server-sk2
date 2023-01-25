#include "messages_control.h"
#include "cJSON_lib/cJSON.h"
#include "hashtable/hashtable.h"
#include "chat_actions.h"
#include "wsServer/ws.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void onopen(ws_cli_conn_t *client);
void onclose(ws_cli_conn_t *client);
void onmessage(ws_cli_conn_t *client, const unsigned char *msg, uint64_t size, int type);
void noop(void *);
ht_iterator_status_t remove_client(hashtable_t *table, void *value, size_t vlen, void *client);

hashtable_t *clients;


void setup_messages()
{
    struct ws_events evs;
    evs.onopen = &onopen;
    evs.onclose = &onclose;
    evs.onmessage = &onmessage;
    clients = ht_create(0, 0, noop);

    ws_socket(&evs, 1101, 1, 1000);
}

void close_messages()
{
    ht_destroy(clients);
}

void onopen(ws_cli_conn_t *client)
{
    char *cli;
    cli = ws_getaddress(client);
    printf("Connection opened, addr: %s\n", cli);
}

void onclose(ws_cli_conn_t *client)
{
    char *cli;
    cli = ws_getaddress(client);
    printf("Connection closed, addr: %s\n", cli);
    ht_foreach_value(clients, remove_client, client);
}

void onmessage(ws_cli_conn_t *client, const unsigned char *msg, uint64_t size, int type)
{
    const char *message = (const char *)msg;
    cJSON *json = cJSON_Parse(message);
    cJSON *operation = cJSON_GetObjectItemCaseSensitive(json, "operation");
    if (strcmp(operation->valuestring, "/start") == 0)
    {
        cJSON *token = cJSON_GetObjectItemCaseSensitive(json, "token");
        int user_id = get_user_id(token->valuestring);
        ws_cli_conn_t **client_p = (ws_cli_conn_t **)malloc(sizeof(ws_cli_conn_t *));
        *client_p = client;
        ht_set(clients, &user_id, sizeof(int), client_p, sizeof(ws_cli_conn_t *));
    }
    else if (strcmp(operation->valuestring, "/message") == 0)
    {
        cJSON *message_json = add_message(json);
        int friend_id = cJSON_GetObjectItemCaseSensitive(message_json, "to")->valueint;
        char *to_send = cJSON_Print(message_json);
        ws_sendframe_txt(client, to_send);
        ws_cli_conn_t **receipent = (ws_cli_conn_t **)ht_get(clients, &friend_id, sizeof(int), NULL);
        if (receipent != NULL)
        {
            ws_sendframe_txt(*receipent, to_send);
        }
        free(to_send);
        cJSON_Delete(message_json);
    }
    cJSON_Delete(json);
}

void noop(void *v)
{
    ws_cli_conn_t **client = (ws_cli_conn_t **)v;
    free(client);
}

ht_iterator_status_t remove_client(hashtable_t *table, void *value, size_t vlen, void *client)
{
    ws_cli_conn_t **stored_client = (ws_cli_conn_t **)value;
    ws_cli_conn_t *to_delete = (ws_cli_conn_t *)client;
    if (*stored_client == to_delete)
    {
        return HT_ITERATOR_REMOVE_AND_STOP;
    }
    return HT_ITERATOR_CONTINUE;
}