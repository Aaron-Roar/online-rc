#include <vector>
#include <string.h>
#include <iostream>
#include <cstdlib>

#include "uiBackend.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int main() {
    uint8_t array[20] = "Aaron";
    copyName(UI.name, array, 0);

    setServerAddress("0.0.0.0", 8000);
    joinServer();

    uint8_t name[] = "Aaron";

    uint8_t dests[] = "Ricky Robot1";
    mutateDestinations(name, '+', '1', dests);

    uint8_t data[] = {1, 0, 8, 8};
    while(true) {
        sendToClients(data, sizeof(data), '1');
    }

}

