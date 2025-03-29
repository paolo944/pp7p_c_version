#include "json_lib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *get_elem(const char *json_obj, const char *key)
{
    char buff[100];
    sprintf(buff, "\"%s\":", key);
    char *start = strstr(json_obj, buff);
    if(start == NULL)
    {
        printf("Erreur: clé json %s non présente\n", key);
        return NULL;
    }
    char *end = strstr(start, ",");
    if(end == NULL)
    {
        end = strstr(start, "}");
        if(end == NULL)
        {
            printf("Erreur pendant le parsing json\n");
            return NULL;
        }
    }
    char *res = (char*)malloc(end-start + 1);
    memcpy(res, start, end-start + 1);
    return res;
}
