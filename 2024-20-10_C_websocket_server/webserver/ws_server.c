#include "server.h"
#include <openssl/evp.h>
#include <openssl/sha.h>

void calc_ws_key(httpM* response, httpM* request);
void init_websocket(httpM* res, httpM* req);

void init_websocket(httpM* response, httpM* request) {
    ws_h ws_h = {};
    response->headers->ws_h = &ws_h;
    calc_ws_key(response, request);
}

void calc_ws_key(httpM* response, httpM* request) {
    char res_key[128] = {};
    char res_key_base64[128];
    char req_key[128] = {};
    char* key_start = strstr(request->message, "Sec-WebSocket-Key:");
    unsigned char sha1_hash[SHA_DIGEST_LENGTH];
    const char* websocket_guid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

    // get key from response
    if (key_start) {
        key_start += strlen("Sec-WebSocket-Key: ");
        char* key_end = strstr(key_start, "\r\n");
        if (key_end) {
            size_t key_length = key_end - key_start;
            strncpy(req_key, key_start, key_length);
            req_key[key_length] = '\0';

            printf("WebSocket Key: %s\n", req_key);
        } else {
            printf("End of key not found.\n");
        }
    } else {
        printf("Sec-WebSocket-Key not found.\n");
    }

    strcat(req_key, "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
    SHA1((unsigned char*)req_key, strlen(req_key), sha1_hash);
    EVP_EncodeBlock((unsigned char*)res_key_base64, sha1_hash,
                    SHA_DIGEST_LENGTH);
    printf("res key = %s\n", res_key_base64);
    strncpy(response->headers->ws_h->ws_key, res_key_base64,
            sizeof(response->headers->ws_h->ws_key) - 1);
    response->headers->ws_h
        ->ws_key[sizeof(response->headers->ws_h->ws_key) - 1] =
        '\0'; // Ensure null-termination
    int msg_len = fillResHeaders(response, request);
    printf("Response message is: %d bytes \n", msg_len);
}
