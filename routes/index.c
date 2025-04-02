#include "index.h"
#include "../src/http_utils.h"
#include "../src/parser.h"
#include <string.h>

void index_route(int client_socket, const char buffer[BUFFER_SIZE])
{
    char *endpoint = get_endpoint(buffer);
    char *c = strstr(buffer, "gzip");
    int gzip = 0 ? c == NULL : 1;

    send_file(client_socket, "public/index.html", "text/html", gzip);
    free(endpoint);
    return;
}

void public_route(int client_socket, const char buffer[BUFFER_SIZE])
{
    char *file = getURL(buffer);
    char *c = strstr(buffer, "gzip");
    int gzip = 0 ? c == NULL : 1;

    char *double_point = strstr(file, "..");
    char *root = strstr(file, "~");

    if(double_point != NULL || root != NULL)
    {
        printf("espèce de petit cachottier\n");
        close(client_socket);
        free(file);
        return;
    }

    char MIME[256];

    if(strstr(file, ".css"))
        snprintf(MIME, sizeof(MIME), "text/css");
    else if(strstr(file, ".js"))
        snprintf(MIME, sizeof(MIME), "application/javascript");
    else if(strstr(file, ".ico"))
        snprintf(MIME, sizeof(MIME), "image/x-icon");

    send_file(client_socket, file, MIME, gzip);
    free(file);
    return;
}
