#ifndef HTTP_CORE_H
#define HTTP_CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

#include "hash_table.h"

#define MAX_CLIENTS 20
#define MAX_LINE_LENGTH 256
#define BUFFER_SIZE 2048

typedef struct {
    int server_port;
    int api_port;
    char api_server[64];
} Config;

extern Config config;

typedef struct {
    int fd;
    int active;
} Client;

extern Client subtitles_clients[MAX_CLIENTS];
extern Client clocks_clients[MAX_CLIENTS];
extern Client teleprompter_clients[MAX_CLIENTS];

typedef struct {
    int client_socket;
}ThreadArgs;

extern pthread_mutex_t lock;

void launch_server();

#endif
