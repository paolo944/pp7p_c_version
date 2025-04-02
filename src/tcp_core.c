#include "http_core.h"
#include <stdio.h>

int connect_to_server()
{
    int sock;
    struct sockaddr_in server_addr;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        return -1;
    }

    // Define server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(config.api_port);
    if (inet_pton(AF_INET, config.api_server, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sock);
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(sock);
        return -1;
    }

    printf("Connected to server at %s:%d\n", config.api_server, config.api_port);
    return sock;
}


char *send_and_receive(int sock, const char *message) //Changer pour voir ce que je recois
{
    char buffer[BUFFER_SIZE];

    send(sock, message, strlen(message), 0);

    int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        char *recv_buff = (char *)malloc((bytes_received + 1)*sizeof(char));
        memcpy(recv_buff, buffer, bytes_received + 1);
        return recv_buff;
    } else {
        printf("No response received or connection closed\n");
        return NULL;
    }
}
