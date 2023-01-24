
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "wsServer/ws.h"

void onopen(ws_cli_conn_t *client)
{
	char *cli;
	char *name;
	cli = ws_getaddress(client); 
	name = ws_getname(client);
	printf("Connection opened, addr:  %s message from: %s \n", cli, name);

}

void onclose(ws_cli_conn_t *client)
{
	char *cli;
	cli = ws_getaddress(client);
	printf("Connection closed, addr: %s\n", cli);
}

void onmessage(ws_cli_conn_t *client,
	const unsigned char *msg, uint64_t size, int type)
{
	char *cli;
	cli = ws_getaddress(client);
	printf("I receive a message: %s (size: %" PRId64 ", type: %d), from: %s\n",
		msg, size, type, cli);
	ws_sendframe(NULL, (char *)msg, size, type);
}


int main(void)
{
	struct ws_events evs;
	evs.onopen    = &onopen;
	evs.onclose   = &onclose;
	evs.onmessage = &onmessage;
	ws_socket(&evs, 8080, 0, 1000); 

	return (0);
}
