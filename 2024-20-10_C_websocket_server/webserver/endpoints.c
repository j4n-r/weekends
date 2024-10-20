#include "server.h"
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <string.h>

int getTasks(httpM* response, httpM* request);
int handleGetRequest(httpM* response, httpM* request);
int postTask(httpM* response, httpM* request);
void calc_ws_key(httpM* response, httpM* request);

void init_websocket(httpM* res, httpM* req);
int routeRequest(httpM* response, httpM* request) {

    // handle GET
    if (request->method == 0) {
        if (strncmp(request->path, "getTasks", PATHBUFSIZE)) {
            getTasks(response, request);
        }
        if (strncmp(request->path, "websocket", PATHBUFSIZE)) {
            init_websocket(response, request);
        }
        handleGetRequest(response, request);
        // handle POST
    } else if (request->method == 1) {
        if (strcmp(request->path, "tasks")) {
            return postTask(response, request);
        }
    }

    return 0;
}

int getTasks(httpM* response, httpM* request) {
    int bytesRead = readFile(response->body, DATABASE_URL);
    printf("Butes read: %d\n", bytesRead);
    for (int i = 0; i < strlen(response->body); i++) {
        if (response->body[i] == '-') {
            response->body[i] = ';';
            continue;
        }
        if (response->body[i] == '[') {
            response->body[i] = ' ';
            continue;
        }
        if (response->body[i] == ']') {
            response->body[i] = ' ';
            continue;
        }
    }

    parseContentTypeFromPath(response, request);
    response->method = POST;
    strcpy(response->path, "/getTasks");
    // construct httpHeaders
    fillReqHeaders(response, request);
    // copy body into message, headers should already be there
    strncat(response->message, response->body, BUFSIZE + HEADERBUFSIZE);
    return 1;
}

int postTask(httpM* response, httpM* request) {
    char tmp[BUFSIZE];
    snprintf(tmp, BUFSIZE - 8, "- [ ] %s\n", request->body);

    strcpy(request->body, tmp);
    writeFile(request->body, DATABASE_URL);
    return 0;
}

void init_websocket(httpM* response, httpM* request) {
    calc_ws_key(response, request);
}

void calc_ws_key(httpM* response, httpM* request) {
    char res_key[48] = {};
    char res_key_base64[64];
    char req_key[48] = {};
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
    strcpy(response->headers->ws_h->ws_key, res_key_base64);
    fillResHeaders(response, request);
}

int handleGetRequest(httpM* response, httpM* request) {
    // read file into body
    response->headers->contentLength = readFile(response->body, request->path);
    // getContentType
    parseContentTypeFromPath(response, request);
    // construct httpHeaders
    fillResHeaders(response, request);
    // copy body into message, headers should already be there
    strncat(response->message, response->body, BUFSIZE + HEADERBUFSIZE);

    return 1;
}
