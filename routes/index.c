#include "index.h"
#include "../src/http_utils.h"
#include "../src/parser.h"
#include <string.h>

void index_route(int client_socket, const char buffer[BUFFER_SIZE])
{
    char *endpoint = get_endpoint(buffer);
    char *c = strstr(buffer, "gzip");
    int gzip = 0 ? c == NULL : 1;

    if(strcmp(endpoint, "") == 0)
        send_file(client_socket, "templates/index.html", "text/html", gzip);
    else if(strcmp(endpoint, "style.min.css") == 0)
        send_file(client_socket, "static/style.min.css", "text/css", gzip);
    else if(strcmp(endpoint, "scripts.min.js") == 0)
        send_file(client_socket, "static/scripts.min.js", "application/javascript", gzip);
    else if(strcmp(endpoint, "favicon.png") == 0)
        send_file(client_socket, "static/favicon.png", "application/javascript", gzip);
    else if(strcmp(endpoint, "installHook.js.map") == 0)
        send_file(client_socket, "static/installHook.js.map", "application/javascript", gzip);
    return;
}
