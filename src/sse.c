#include "sse.h"

void send_sse_message(Client *clients, int client_count, const char *message) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "data: %s\n\n", message);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].active) {
            if (send(clients[i].fd, buffer, strlen(buffer), MSG_NOSIGNAL) == -1) {
                perror("send");
                close(clients[i].fd);
                clients[i].active = 0;
            }
        }
    }
}
