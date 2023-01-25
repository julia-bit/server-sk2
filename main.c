#include "cJSON_lib/cJSON.h"
#include "operations.h"
#include "messages_control.h"
#include "requests.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static int serverSocket;

void int_handler(int sign)
{
    shutdown(serverSocket, SHUT_RD);
    close(serverSocket);
    close_messages();
}

void *socketThread(void *arg)
{
    int *socket = (int *)arg;

    request *request = get_request(*socket);
    if (!request)
    {
        printf("closing socket %d\n", *socket);
        close(*socket);
        free(socket);
        pthread_exit(NULL);
    }
    printf("%s\n", request->method);
    printf("%s\n", request->path);

    if (request->has_data == true)
    {
        printf("%lu %s\n", strlen(request->data), request->data);
    }

    char *response = handle_request(request); 
    send(*socket, response, strlen(response), 0);

    close(*socket);
    destroy_request(request);
    free(response);
    free(socket);

    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    signal(SIGINT, int_handler);
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    const int one = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1100);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    if (listen(serverSocket, 50) == 0)
        printf("Listening\n");
    else
        printf("Error\n");
    pthread_t thread_id;

    setup_messages();

    while (1)
    {
        addr_size = sizeof serverStorage;
        int *newSocket = malloc(1 * sizeof(int));
        if (!newSocket)
        {
            fprintf(stderr, "Memory allocation failed!\n");
            continue;
        }
        *newSocket = accept(serverSocket, (struct sockaddr *)&serverStorage, &addr_size);
        if (*newSocket == -1)
        {
            free(newSocket);
            break;
        }
        if (pthread_create(&thread_id, NULL, socketThread, newSocket) != 0)
            printf("Failed to create thread\n");

        pthread_detach(thread_id);
    }
    return 0;
}
