#include "chat_actions.h"
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>
#include <stdlib.h>

cJSON *get_users(cJSON *request, char *user_id, http_status *response_status)
{
    sqlite3 *db;
    sqlite3_open("chat-sk.db", &db);
    sqlite3_stmt *stmt;

    char *sql = "SELECT user_id, username FROM users";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    cJSON *response_json = cJSON_CreateArray();

    if(stmt!= NULL)
    {
        while(sqlite3_step(stmt) != SQLITE_DONE)
        {
            cJSON *user_json = cJSON_CreateObject();
            cJSON_AddStringToObject(user_json, "user_id", sqlite3_column_text(stmt, 0));
            cJSON_AddStringToObject(user_json, "username", sqlite3_column_text(stmt, 1));
            cJSON_AddItemToArray(response_json, user_json);
        }
    
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
    *response_status = HTTP_OK;
    return response_json;
}

cJSON *add_user(cJSON *request, char *user_id)
{
    sqlite3 *db;
    sqlite3_open("chat-sk.db", &db);
    sqlite3_stmt *stmt;
    cJSON *username = cJSON_GetObjectItemCaseSensitive(request, "username");
    char *sql = "INSERT INTO users VALUES(?, ?)";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        sqlite3_bind_text(stmt, 1, user_id,-1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, username->valuestring, -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    sqlite3_close(db);
    return NULL;
}

cJSON *add_message(cJSON *request)
{
    cJSON *from = cJSON_GetObjectItemCaseSensitive(request, "from");
    cJSON *to = cJSON_GetObjectItemCaseSensitive(request, "to");
    cJSON *message = cJSON_GetObjectItemCaseSensitive(request, "message");
    cJSON *timestamp = cJSON_GetObjectItemCaseSensitive(request, "timestamp");

    char *user_id = from->valuestring;
    char *friend_id = to->valuestring;
    char *message_str = message->valuestring;
    char *timestamp_str = timestamp->valuestring;

    sqlite3 *db;
    sqlite3_open("chat-sk.db", &db);
    sqlite3_stmt *stmt;

    char *sql = "INSERT INTO messages(sender_id, receiver_id, message, sending_date) VALUES(?, ?, ?, ?)";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if(stmt != NULL) {
        sqlite3_bind_text(stmt, 1, user_id,-1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, friend_id,-1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, message_str, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, timestamp_str, -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    sqlite3_stmt *stmt2;
    char *sql2 = "SELECT message_id FROM messages WHERE sender_id = ? AND receiver_id = ? AND message LIKE ? AND sending_date LIKE ?";
    sqlite3_prepare_v2(db, sql2, -1, &stmt2, 0);
    int message_id = 0;

    if(stmt2 != NULL) {
        sqlite3_bind_text(stmt2, 1, user_id,-1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt2, 2, friend_id,-1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt2, 3, message_str, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt2, 4, timestamp_str, -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        message_id = sqlite3_column_int(stmt2, 0);
        sqlite3_finalize(stmt);
    }


    cJSON *message_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(message_json, "id", message_id);
    cJSON_AddStringToObject(message_json, "from", user_id);
    cJSON_AddStringToObject(message_json, "to", friend_id);
    cJSON_AddStringToObject(message_json, "message", message_str);
    cJSON_AddStringToObject(message_json, "timestamp", timestamp_str);

    sqlite3_close(db);
    return message_json;
}


cJSON *get_messages(cJSON *request, char* sender_id, http_status *response_status)
{

    cJSON *receiver_id = cJSON_GetObjectItemCaseSensitive(request, "receiver_id");

    sqlite3 *db;
    sqlite3_open("chat-sk.db", &db);
    sqlite3_stmt *stmt;

    char *sql = "SELECT * FROM messages WHERE (sender_id = ? AND receiver_id = ?) OR (receiver_id = ? AND sender_id = ?)";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    cJSON *response_json = cJSON_CreateArray();
    if(stmt != NULL){
        sqlite3_bind_text(stmt, 1, receiver_id -> valuestring,-1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, sender_id,-1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, sender_id,-1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, receiver_id -> valuestring,-1, SQLITE_TRANSIENT);
        while (sqlite3_step(stmt) != SQLITE_DONE){
            cJSON *message_json = cJSON_CreateObject();
            cJSON_AddNumberToObject(message_json, "id", sqlite3_column_int(stmt, 0));
            cJSON_AddStringToObject(message_json, "sender_id", sqlite3_column_text(stmt, 1));
            cJSON_AddStringToObject(message_json, "receiver_id", sqlite3_column_text(stmt, 2));
            cJSON_AddStringToObject(message_json, "message", sqlite3_column_text(stmt, 3));
            cJSON_AddStringToObject(message_json, "timestamp", sqlite3_column_text(stmt, 4));
            cJSON_AddItemToArray(response_json, message_json);
        }
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
    *response_status = HTTP_OK;
    return response_json;
}