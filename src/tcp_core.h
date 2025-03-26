#ifndef TCP_CORE_H
#define TCP_CORE_H

int connect_to_server();

char *send_and_receive(int sock, const char *message);

#endif
