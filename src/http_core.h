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

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 20
#define PORT 8080
#define PP7_IP "127.0.0.1"
#define PP7_PORT 9000

typedef struct {
    int fd;
    int active;
} Client;

extern Client subtitles_clients[MAX_CLIENTS];
extern Client clocks_clients[MAX_CLIENTS];
extern Client teleprompter_clients[MAX_CLIENTS];

typedef struct {
    int client_socket;
    HashTable *table;
}ThreadArgs;

extern pthread_mutex_t lock;

void launch_server(HashTable *table);

#endif
