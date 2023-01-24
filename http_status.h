#ifndef HTTP_STATUS_H
#define HTTP_STATUS_H

typedef int http_status;

#define HTTP_OK ((http_status)1)
#define HTTP_BAD_REQUEST ((http_status)2)
#define HTTP_DISALLOWED_METHOD ((http_status)3)
#define HTTP_UNAUTHORIZED ((http_status)4)
#define HTTP_NOT_FOUND ((http_status)5)
#define HTTP_INTERNAL_SERVER_ERROR ((http_status)6)

#endif