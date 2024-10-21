#ifndef WS_SERVER
#define WS_SERVER
#include <openssl/evp.h>
#include <openssl/sha.h>
#include "server.h"

typedef struct dataframe {
	unsigned char flags;
	unsigned char payload_len;
	unsigned char ext_payload_len;
	char payload[4096];

} ws_df;
void calc_ws_key(httpM *response, httpM *request);
void init_websocket(httpM *res, httpM *req);
void send_ws_message(char *message, int fd_socket);
#endif
