#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


struct Header {
    int type;
    Header* next;
};

struct MsgImg {
    struct Header Head;
    uint8_t data;
};

struct MsgCon {
    struct Header Head;
    uint8_t data;
};

union Payload {
    struct MsgCon cons;
    struct MsgImg Img;
};

struct ServerMsg {
    uint64_t size;
    uint8_t type;
    union Payload payload;
};

int sockfd;

int main() {
    socklen_t socklength;
    struct MsgImg Img;
    Img.data = 26;
    Img.size = sizeof(Img);

    struct sockaddr_in Client;
    Client.sin_port = htons(9000);
    Client.sin_addr.s_addr = inet_addr("0.0.0.0");
    Client.sin_family = AF_INET;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sockfd, (struct sockaddr*)&Client, sizeof(Client));

    recvfrom(sockfd, &D1.size, sizeof(D1.size), MSG_PEEK, (struct sockaddr*)&Client, &socklength);
    printf("Peeked size was: %d\n", D1.size);
    recvfrom(sockfd, &D1, D1.size, 0, (struct sockaddr*)&Client, &socklength);
    printf("Value: %d\n", D1.value);
}

