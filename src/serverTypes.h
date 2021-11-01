#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "hashTableTypes.h"

namespace Server {
    struct Client {
        uint8_t name[20];
        uint8_t uid;
        int temperature;
        struct sockaddr_in Address;
        socklen_t len;

        Hash::Table<Hash::Table<Client*, char> , char> DestTable;
    };

};
