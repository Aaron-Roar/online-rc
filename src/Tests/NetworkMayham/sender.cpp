#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


struct Data {
    uint8_t size;
    uint8_t value;
};

int sockfd;

int main() {
    struct Data D1;
    D1.value = 26;
    D1.size = sizeof(D1);

struct sockaddr_in Client;
Client.sin_port = htons(9000);
Client.sin_addr.s_addr = inet_addr("0.0.0.0");
Client.sin_family = AF_INET;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

sendto(sockfd, (char*)&D1, D1.size, 0, (struct sockaddr*)&Client, sizeof(Client));
}


