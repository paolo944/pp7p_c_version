#ifndef SSE_H
#define SSE_H

#include "http_core.h"

void send_sse_message(Client *clients, int client_count, const char *message);

#endif