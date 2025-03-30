#include "index.h"
#include "../src/http_utils.h"
#include "../src/parser.h"
#include <string.h>

void index_route(int client_socket, const char buffer[BUFFER_SIZE])
{
    char *endpoint = get_endpoint(buffer);
    char *c = strstr(buffer, "gzip");
    int gzip = 0 ? c == NULL : 1;

    if(gzip == 0)
    {
        if(strcmp(endpoint, "") == 0)
            send_file(client_socket, "templates/index.html", "text/html", gzip);
        else if(strcmp(endpoint, "style.css") == 0)
            send_file(client_socket, "static/style.css", "text/css", gzip);
        else if(strcmp(endpoint, "scripts.js") == 0)
            send_file(client_socket, "static/scripts.js", "application/javascript", gzip);
        else if(strcmp(endpoint, "favicon.ico") == 0)
            send_file(client_socket, "static/favicon.ico", "application/javascript", gzip);
        else if(strcmp(endpoint, "installHook.js.map") == 0)
            send_file(client_socket, "static/installHook.js.map", "application/javascript", gzip);
    }
    else
    {
        if(strcmp(endpoint, "") == 0)
            send_file(client_socket, "templates/index.html.gz", "text/html", gzip);
        else if(strcmp(endpoint, "style.css") == 0)
            send_file(client_socket, "static/style.css.gz", "text/css", gzip);
        else if(strcmp(endpoint, "scripts.js") == 0)
            send_file(client_socket, "static/scripts.js.gz", "application/javascript", gzip);
        else if(strcmp(endpoint, "favicon.ico") == 0)
            send_file(client_socket, "static/favicon.ico.gz", "application/javascript", gzip);
        else if(strcmp(endpoint, "installHook.js.map") == 0)
            send_file(client_socket, "static/installHook.js.map.gz", "application/javascript", gzip);

    }
    close(client_socket);
    return;
}
