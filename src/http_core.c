#include "http_utils.h"
#include "http_core.h"
#include "hash_table.h"

void launch_server(HashTable *table)
{
    int server_fd, client_addr_len, epoll_fd, nfds;
	struct sockaddr_in client_addr;

	struct epoll_event ev, events[MAX_CLIENTS];

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		printf("Socket creation failed: %s...\n", strerror(errno));
		return;
	}

	set_nonblocking(server_fd);

	struct sockaddr_in serv_addr = { .sin_family = AF_INET ,
									 .sin_port = htons(PORT),
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

	// Setup epoll
    epoll_fd = epoll_create1(0);
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);

    ev.data.fd = tcp_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tcp_fd, &ev);

	printf("Waiting for a client to connect...\n");
	client_addr_len = sizeof(client_addr);

	while(1)
	{
		int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, (socklen_t * restrict)&client_addr_len);

		if(client_fd == -1)
			continue;

		pthread_t new_process;
    	ThreadArgs *args = malloc(sizeof(ThreadArgs));
    	args->client_socket = client_fd;
    	args->table = table;
    	pthread_create(&new_process, NULL, handle_connection, args);
	}

	close(server_fd);
    close(epoll_fd);

    return;
}

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int connect_to_pp7() {
    int tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in tcp_server_addr;
    
    tcp_server_addr.sin_family = AF_INET;
    tcp_server_addr.sin_port = htons(PP7_PORT);
    inet_pton(AF_INET, PP7_IP, &tcp_server_addr.sin_addr);

    while (connect(tcp_fd, (struct sockaddr *)&tcp_server_addr, sizeof(tcp_server_addr)) == -1) {
        perror("TCP connection failed, retrying in 5s...");
        close(tcp_fd);
        sleep(5);
        tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
    }

    set_nonblocking(tcp_fd);
    return tcp_fd;
}

    tcp_fd = connect_to_tcp_server();

    while (1) {
        nfds = epoll_wait(epoll_fd, events, MAX_CLIENTS, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_fd) {
                // New SSE client
                int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
                set_nonblocking(client_fd);
                ev.events = EPOLLIN | EPOLLOUT;
                ev.data.fd = client_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);

                // Read HTTP request to determine endpoint
                char request[BUFFER_SIZE];
                read(client_fd, request, sizeof(request) - 1);
                printf("Client request: %s\n", request);

                // Assign client to appropriate list
                if (strstr(request, "GET /sse/sports")) {
                    sports_clients[sports_count++] = (Client){client_fd, 1};
                } else if (strstr(request, "GET /sse/news")) {
                    news_clients[news_count++] = (Client){client_fd, 1};
                }

                // Send SSE headers
                const char *headers = "HTTP/1.1 200 OK\r\n"
                                      "Content-Type: text/event-stream\r\n"
                                      "Cache-Control: no-cache\r\n"
                                      "Connection: keep-alive\r\n\r\n";
                send(client_fd, headers, strlen(headers), 0);
            } else if (events[i].data.fd == tcp_fd) {
                // Read from TCP server
                char buffer[BUFFER_SIZE];
                int bytes_read = read(tcp_fd, buffer, sizeof(buffer) - 1);

                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0';
                    printf("Received: %s\n", buffer);

                    // Simple filtering logic (Modify based on actual data)
                    if (strstr(buffer, "SPORTS:")) {
                        send_sse_message(sports_clients, sports_count, buffer);
                    } else if (strstr(buffer, "NEWS:")) {
                        send_sse_message(news_clients, news_count, buffer);
                    }
                } else if (bytes_read == 0) {
                    printf("TCP server disconnected, reconnecting...\n");
                    close(tcp_fd);
                    tcp_fd = connect_to_tcp_server();
                    ev.data.fd = tcp_fd;
                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tcp_fd, &ev);
                }
            }
        }
    }