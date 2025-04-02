#include "http_utils.h"
#include "http_core.h"
#include "hash_table.h"
#include <signal.h>
#include <ifaddrs.h>

int server_fd;

Config config;

void getServerIP(char *ipBuffer, size_t bufferSize) {
    struct ifaddrs *ifaddr, *ifa;
    int family;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return;
    }

    // Parcourir toutes les interfaces réseau
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;

        family = ifa->ifa_addr->sa_family;

        // Vérifier si c'est une adresse IPv4
        if (family == AF_INET) {
            struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
            const char *ip = inet_ntop(AF_INET, &addr->sin_addr, ipBuffer, bufferSize);
            
            // Exclure l'interface loopback "127.0.0.1"
            if (ip && strcmp(ipBuffer, "127.0.0.1") != 0) {
                break;  // On prend la première IP non-loopback trouvée
            }
        }
    }

    freeifaddrs(ifaddr);
}

int loadConfig(){
    FILE *file = fopen(".env", "r");
    if (!file) {
        perror("Erreur ouverture du fichier de configuration");
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char key[MAX_LINE_LENGTH], value[MAX_LINE_LENGTH];

        if (sscanf(line, "%[^=]=%s", key, value) == 2) {
            if (strcmp(key, "server_port") == 0) {
                config.server_port = atoi(value);
            } else if (strcmp(key, "api_port") == 0) {
                config.api_port = atoi(value);
            } else if (strcmp(key, "api_server") == 0) {
                strncpy(config.api_server, value, sizeof(config.api_server));
            }
        }
    }

    fclose(file);
    return 1;
}

void handle_signal(int signal)
{
    printf("\n Received signal %d, Stopping the server\n", signal);

    if(server_fd != -1)
    {
        close(server_fd);
        printf("Server socket closed\n");
    }

	freeHashTable();
    exit(0);
}

void launch_server()
{
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    loadConfig();

    int client_addr_len;
	struct sockaddr_in client_addr;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		printf("Socket creation failed: %s...\n", strerror(errno));
		return;
	}

	struct sockaddr_in serv_addr = { .sin_family = AF_INET ,
									 .sin_port = htons(config.server_port),
									 .sin_addr = { htonl(INADDR_ANY) },
									};

	if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0)
	{
		printf("Bind failed: %s \n", strerror(errno));
		return;
	}

	if (listen(server_fd, MAX_CLIENTS) != 0)
	{
		printf("Listen failed: %s \n", strerror(errno));
		return;
	}

    char serverIP[INET_ADDRSTRLEN] = {0};

    getServerIP(serverIP, sizeof(serverIP));

    if(serverIP[0] == '\0'){
        printf("Couldn't get the server IP adress\n");

        if(server_fd != -1)
        {
            close(server_fd);
            printf("Server socket closed\n");
        }

        freeHashTable();
        exit(0);
    }

    printf("Server listening on http://%s:%d\n", serverIP, config.server_port);
    client_addr_len = sizeof(client_addr);

    while(1)
    {
        int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, (socklen_t * restrict)&client_addr_len);

        if(client_fd == -1)
            continue;

        pthread_t new_process;
        ThreadArgs *args = malloc(sizeof(ThreadArgs));
        args->client_socket = client_fd;
        pthread_create(&new_process, NULL, handle_connection, args);
    }

    close(server_fd);
    return;
}

int connect_to_pp7() {
    int tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in tcp_server_addr;

    tcp_server_addr.sin_family = AF_INET;
    tcp_server_addr.sin_port = htons(config.api_port);
    inet_pton(AF_INET, config.api_server, &tcp_server_addr.sin_addr);

    while (connect(tcp_fd, (struct sockaddr *)&tcp_server_addr, sizeof(tcp_server_addr)) == -1) {
        perror("TCP connection failed, retrying in 5s...");
        close(tcp_fd);
        sleep(5);
        tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
    }

    return tcp_fd;
}
