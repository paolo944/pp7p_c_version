#ifndef INDEX_H
#define INDEX_H

#include "../src/http_core.h"

void index_route(int client_socket, const char buffer[BUFFER_SIZE]);

void public_route(int client_socket, const char buffer[BUFFER_SIZE]);

#endif
