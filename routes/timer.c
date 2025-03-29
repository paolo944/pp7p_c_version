#include "timer.h"
#include "../src/tcp_core.h"
#include "../src/json_lib.h"
#include "../src/parser.h"
#include <stdio.h>

static void timer_action(int client_socket, const char *uuid, const char *action)
{
    char request[1024];
    sprintf(request, "{\"url\":\"v1/timer/%s/%s\",\"method\":\"GET\"}", uuid, action);

    int pp7 = connect_to_server();

    char *buff = send_and_receive(pp7, request);
    char response[BUFFER_SIZE];
    if(buff != NULL){
        fprintf(stderr, "%s\n", buff);
        sprintf(response, "HTTP/1.1 400 Bad request\r\n\r\n");
    }
    else{
        sprintf(response, "HTTP/1.1 200\r\n\r\n");
    }
    send(client_socket, response, strlen(response), 0);
    close(client_socket);
    return;
}

static void timer_delete_pp7(int client_socket, const char *uuid)
{
    char request[1024];
    sprintf(request, "{\"url\":\"v1/timer/%s\",\"method\":\"DELETE\"}", uuid);

    int pp7 = connect_to_server();
    char *buff = send_and_receive(pp7, request);
    char response[BUFFER_SIZE];
    if(buff != NULL){
        fprintf(stderr, "%s\n", buff);
        sprintf(response, "HTTP/1.1 400 Bad request\r\n\r\n");
    }
    else{
        sprintf(response, "HTTP/1.1 200\r\n\r\n");
    }
    send(client_socket, response, strlen(response), 0);
    close(client_socket);
    return;
}

static void timer_add(int client_socket, const char *data)
{
    char body[400];
    char request[1024];

    char *hours_str = get_elem(data, "hours");
    char *minutes_str = get_elem(data, "minutes");
    char *seconds_str = get_elem(data, "seconds");

    int hours = atoi(hours_str);
    int minutes = atoi(minutes_str);
    int seconds = atoi(seconds_str);
    char *name = get_elem(data, "name");
    seconds += hours*3600 + minutes*60;

    sprintf(body, "{\"allows_overrun: True,\"countdown\":{\"duration\":%d},\"name\":\"%s\"}", seconds, name); 

    sprintf(request, "{\"url\":\"v1/timers\",\"method\":\"POST\",\"body\":%s}", body);

    int pp7 = connect_to_server();
    char *buff = send_and_receive(pp7, request);
    char response[BUFFER_SIZE];
    if(buff == NULL){
        fprintf(stderr, "Erreur pendant l'ajout de la clock\n");
        sprintf(response, "HTTP/1.1 400 Bad request\r\n\r\n");
    }
    else{
        int code = getCode(buff); 
        if(code == 200){
            char *body = getBody(buff);
            char *uuid = get_elem(body, "uuid");
            sprintf(response, "HTTP/1.1 200 OK\r\n%s\r\n\r\n", uuid);
        }
        else{
            sprintf(response, "HTTP/1.1 400 Bad request\r\n\r\n");
        }
    }

    send(client_socket, response, strlen(response), 0);
    close(client_socket);
    return;
}

void timer_handler(int client_socket, const char *buffer)
{
    char *method = getMethod(buffer);
    if(strcmp(method, "GET") == 0)
    {
        const char *body = getBody(buffer);
        const char *uuid = get_elem(body, "uuid");
        const char *action = get_elem(body, "action");
        timer_action(client_socket, uuid, action);
    }
    if(strcmp(method, "DELETE") == 0)
    {
        const char *body = getBody(buffer);
        const char *uuid = get_elem(body, "uuid");
        timer_delete_pp7(client_socket, uuid);
    }
    if(strcmp(method, "PUT") == 0)
    {
        const char *body = getBody(buffer);
        timer_add(client_socket, body);
    }
    else
    {
        fprintf(stderr, "Erreur, pas de méthode %s pour timer\n", method);
        char response[BUFFER_SIZE];
        sprintf(response, "HTTP/1.1 400 Bad request\r\n\r\n");
        send(client_socket, response, strlen(response), 0);
        close(client_socket);
    }
    return;
}

