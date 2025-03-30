#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

#include <unistd.h>
#include "http_core.h"

void *handle_connection(void *arg);

void send_file(int client_socket, const char *fn, const char *content_type, int gzip, int keep_alive);

#endif
