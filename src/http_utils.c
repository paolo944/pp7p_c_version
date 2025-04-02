#include "http_utils.h"
#include "routing.h"
#include <sys/sendfile.h>
#include <sys/stat.h>

void *handle_connection(void *arg)
{
	ThreadArgs *args = (ThreadArgs*)arg;
	int client_socket = args->client_socket;

	char buffer[BUFFER_SIZE] = {0};	

	int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

	if(bytes_received == 0){
		printf("J'ai rien reçu!\n");
		return NULL;
	}

	route(buffer, client_socket);

    free(arg);

	return NULL;
}

void send_file(int client_socket, const char *fn, const char *MIME, int gzip)
{
    char fnp[100];
    if (gzip)
    {
        snprintf(fnp, sizeof(fnp), "%s.gz", fn);  // Ajoute .gz si gzip est activé
    } 
    else
    {
        snprintf(fnp, sizeof(fnp), "%s", fn);  // Sinon, garde le nom normal
    }
    int f = open(fnp, O_RDONLY);
    if(f == -1)
    {
        fprintf(stderr, "didn't find %s\n", fn);
        const char *error_message = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(client_socket, error_message, strlen(error_message), 0);
        return ;
    }

    struct stat st;
    if(fstat(f, &st) == -1)
    {
        perror("Error getting file size");
        close(f);
        return;
    }

    char header[256];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "%s"
             "Content-Length: %ld\r\n"
             "Connection: close\r\n\r\n",
             MIME,
             gzip ? "Content-Encoding: gzip\r\n": "", 
             st.st_size);
    send(client_socket, header, strlen(header), 0);

    off_t offset = 0;
    ssize_t sent_bytes = sendfile(client_socket, f, &offset, st.st_size);
    if(sent_bytes == -1)
        perror("Error sending file");
    close(f);
    close(client_socket);
    return;
}
