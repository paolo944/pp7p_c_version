#include "timer.h"
#include "../src/tcp_core.h"
#include <cjson/cJSON.h>
                    
void play(pp7, uuid)
{
    char *url = "v1/timer/%s/play";

    cJSON *request = cJSON_CreateObject(); 
    cJSON_AddStringToObject(request, "url", url); 
    char *request_str = cJSON_PrintUnformatted(request); //Changer pour ajouter le \r\n à la fin et adapter

    int pp7 = connect_to_server();

    send_and_receive(pp7, request_str)

    //Si je reçois rien alors c'est bon et j'envoio au client un http avec code 204

        if response.status_code == 204:
            return True
        else:
            printf(f'Échec de la requête. Code de statut : {response.status_code}')

    free(request_str);
    cJSON_Delete(request);
}
    def pause(self, uuid):
        headers = {
            'Content-Type': 'application/json',
            'accept': '*/*'
        }

        response = requests.get(f"{self.url}/{uuid}/stop", headers=headers)

        if response.status_code == 204:
            print(f"Clock {uuid} stopped")
            return True
        else:
            print(f'Échec de la requête. Code de statut : {response.status_code}')
            return False

    def reset(self, uuid):
        headers = {
            'Content-Type': 'application/json',
            'accept': '*/*'
        }

        response = requests.get(f"{self.url}/{uuid}/reset", headers=headers)

        if response.status_code == 204:
            print(f"Clock {uuid} reset")
            return True
        else:
            print(f'Échec de la requête. Code de statut : {response.status_code}')
            return False

    def delete(self, uuid):
        headers = {
            'Content-Type': 'application/json',
            'accept': '*/*'
        }

        response = requests.delete(f"{self.url}/{uuid}", headers=headers)

        if response.status_code == 204:
            print(f"Clock {uuid} deleted")
            return True
        else:
            print(f'Échec de la requête. Code de statut : {response.status_code}')
            return False

    def post(self, data):
        headers = {
            'Content-Type': 'application/json',
            'accept': '*/*'
        }

        hours = data.get('hours')
        minutes = data.get('minutes')
        seconds = data.get('seconds')
        name = data.get('clock_name')

        seconds = int(seconds)
        seconds += int(minutes) * 60
        seconds += int(hours) * 3600

        data = {
            "allows_overrun": True,
            "countdown": {
                "duration": seconds
            },
            "name": name
        }

        json_data = json.dumps(data)

        response = requests.post(f"{self.url}s", headers=headers, data=json_data)

        if response.status_code == 200:
            print(f"Clock {name} ajouté")
            return True
        else:
            print(f'Échec de la requête. Ajout clock, Code de statut : {response.status_code}')
            return False
    
    