#include "../../uiBackend.cpp"


int main() {
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8000);
    server.sin_addr.s_addr = inet_addr("0.0.0.0");


    Client client;
    client.setTest(1);

    uint8_t name[20] = "Aaron";
    uint8_t bio[20] = "Mechatronics";

    client.setServerAddress(server);
    client.join(name, bio);
    if(client.uid == 0) {
        perror("Failed to be added by server\n");
    }
    client.mutateDests(client.uid, 1, &client.uid, '+');
    client.leave();
}
