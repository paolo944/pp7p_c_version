#include "http_core.h"

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
    server_addr.sin_port = htons(PP7_PORT);
    if (inet_pton(AF_INET, PP7_IP, &server_addr.sin_addr) <= 0) {
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

    printf("Connected to server at %s:%d\n", PP7_IP, PP7_PORT);
    return sock;
}


void send_and_receive(int sock, const char *message) //Changer pour voir ce que je recois
{
    char buffer[BUFFER_SIZE];

    send(sock, message, strlen(message), 0);
    printf("Sent: %s\n", message);

    int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("Server response: %s\n", buffer);
    } else {
        printf("No response received or connection closed\n");
    }
}
