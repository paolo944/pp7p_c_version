#include "timer.h"
#include "../src/tcp_core.h"
#include <stdio.h>

void timer_action(const char *uuid, const char *action)
{
    char request[1024];
    sprintf(request, "{\"url\":\"v1/timer/%s/%s\",\"method\":\"GET\"}", uuid, action);

    int pp7 = connect_to_server();

    char *buff = send_and_receive(pp7, request);
    if(buff == NULL)
        return;
    else{
        fprintf(stderr, "%s\n", buff);
    }
}

void timer_delete(const char *uuid)
{
    char request[1024];
    sprintf(request, "{\"url\":\"v1/timer/%s\",\"method\":\"DELETE\"}", uuid);

    int pp7 = connect_to_server();
    char *buff = send_and_receive(pp7, request);
    if(buff == NULL)
        return;
    else{
        fprintf(stderr, "%s\n", buff);
    }
}

void timer_add(const char *data)
{
    char body[400];
    char request[1024];

    int hours;
    int minutes;
    int seconds;
    char *name;
    seconds += hours*3600 + minutes*60;

    sprintf(request, "{\"url\":\"v1/timers\",\"method\":\"POST\",\"body\":%s}", body);
    
    int pp7 = connect_to_server();
    char *buff = send_and_receive(pp7, request);
    if(buff == NULL){
        fprintf(stderr, "Erreur pendant l'ajout de la clock\n");
        return;
    }
    else{
        return;
    }
}
