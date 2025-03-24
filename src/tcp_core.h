#ifndef TCP_CORE_H
#define TCP_CORE_H

int connect_to_server();

void send_and_receive(int sock, const char *message);

#endif