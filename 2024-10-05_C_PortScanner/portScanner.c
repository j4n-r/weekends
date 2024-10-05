#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <bits/types/struct_iovec.h>
#include <bits/types/struct_timeval.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Usage: %s <IPv4 Address> <Port | Port-Port> <TCP || UDP>",
               argv[0]);
        exit(1);
    }
    int socktype = 0;

    char* port_start_str = argv[2];
    char* port_end_str = {};
    int port_start = 0;
    int port_end = 0;
    if (strstr(argv[2], "-")) {
        port_start_str = strtok(argv[2], "-");
        port_end_str = strtok(NULL, "-");
        port_start = atoi(port_start_str);
        port_end = atoi(port_end_str);
    } else {
        port_start = atoi(argv[2]);
    }
    int socketfd;
    char buf[4086];
    struct sockaddr_in scan_addr = {};
    struct sockaddr_in recv_addr_info = {};
    struct timeval timeout = {};
    unsigned int b_inet_addr = 0;

    if ((int)*argv[1] > 65535) {
        printf("invalid Portnumber");
        exit(1);
    }
    if (inet_pton(AF_INET, argv[1], &b_inet_addr) == 0) {
        printf("Address is no valid");
        exit(1);
    }
    if (!strncmp(argv[3], "TCP", 3)) {
        socktype = SOCK_STREAM;
    }
    if (!strncmp(argv[3], "UDP", 3)) {
        socktype = SOCK_DGRAM;
    }

    scan_addr.sin_family = AF_INET;
    scan_addr.sin_addr.s_addr = b_inet_addr;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100;

    if (socktype == SOCK_STREAM) {
        for (int port = port_start; port < port_end; port++) {
            socketfd = socket(AF_INET, socktype, 0);
            if (socketfd == -1) {
                perror("socket error");
                exit(1);
            }
            scan_addr.sin_port = htons(port);
            if (connect(socketfd, (struct sockaddr*)&scan_addr,
                        sizeof(scan_addr)) == -1) {
            } else {
                printf("Port: %d is open\n", port);
            }
            close(socketfd);
        }
    }

    /*if (socktype == SOCK_DGRAM) {*/
    /*    for (int port = port_start; port < port_end; port++) {*/
    /*        socketfd = socket(AF_INET, socktype, 0);*/
    /*        if (socketfd == -1) {*/
    /*            perror("socket error");*/
    /*            exit(1);*/
    /*        }*/
    /**/
    /*        if (setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, &timeout,*/
    /*                       sizeof(timeout)) < 0) {*/
    /*            perror("setsockopt failed");*/
    /*            close(socketfd);*/
    /*            continue;*/
    /*        }*/
    /*        scan_addr.sin_port = htons(port);*/
    /*        if (connect(socketfd, (struct sockaddr*)&scan_addr,*/
    /*                    sizeof(scan_addr)) == -1) {*/
    /*            perror("Connection failed \n");*/
    /*            close(socketfd);*/
    /*            continue;*/
    /*        }*/
    /*        size_t bytes_send = send(socketfd, buf, strlen(buf), 0);*/
    /*        size_t bytes_recv = recv(socketfd, buf, strlen(buf), 0);*/
    /*        if (bytes_recv == -1) {*/
    /*            if (errno == ECONNREFUSED) {*/
    /*                printf("Port: %d open \n ", port);*/
    /*            }*/
    /*            if (errno == EAGAIN || errno == EWOULDBLOCK) {*/
    /*            }*/
    /*        } else {*/
    /*            printf("bytes received: %zd from port %d\nbuf: %s\n",*/
    /*                   bytes_recv, port, buf);*/
    /*        }*/
    /*        close(socketfd);*/
    /*    }*/
    /*}*/

    close(socketfd);
    exit(1);
}
