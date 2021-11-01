#include "../../../RoboCMD/RoboCMD.h"

int main() {
    struct sockaddr_in Address;
    socklen_t addr_len;
    Address.sin_family = AF_INET;
    Address.sin_port = htons(8000);
    Address.sin_addr.s_addr = inet_addr("0.0.0.0");
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    uint8_t array[20] = "Mike";
    copyName(UI.name, array, 0);

    setServerAddress("0.0.0.0", 8000);
    joinServer();

    int i = 0;
    while(true) {
        printf("\nMessage: %d\n", i);
    RoboCMD <float, float, float, float> cmd;
    cmd.setMsg(recvFromClient());
    cmd.disMsg();
    printf("\n\n");
    i += 1;
    }
}



