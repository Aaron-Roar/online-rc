    //u_int8_t mutation[] = " 3 + Bob:1 Ricky Jeff John \0\0\0";
    //uint8_t data[7] = {6, '2', ' ', uid, '1', 25, '\0'};

#include "uiBackend.h"

struct sockaddr_in ServerAddress;
int sock = socket(AF_INET, SOCK_DGRAM, 0);

void writeToSS(uint8_t* msg, uint64_t size, std::stringstream& sd) {
    int i = 0;
    while(i < size) {
        sd << msg[i];
        i += 1;
    }
}

void printMsg(const uint8_t* data, uint64_t size) {
    printf("\n");
    int i = 0;
    while(i < size) {
        if(data[i] == '\0') {
            //printf("[NULL]");
        }
        else {
        printf("%c", data[i]);
        }
            i += 1;
    }
    printf("\n");
}
void printVector(std::vector<uint8_t> vec) {
    int i = 0;
    printf("Vector: ");
    while(i < vec.size()) {
        printf("%d, ", vec[i]);
        i += 1;
    }

}

void setServerAddress(const char* ip, int port) {
    ServerAddress.sin_port = htons(port);
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.s_addr = inet_addr(ip);
}

struct Node UI;

void mutateDestinations(uint8_t* name, uint8_t sign, uint8_t op, uint8_t* destinations) {
    std::stringstream ss;
    ss << "3 " << sign << ' ' << name << ':' << op << ' ' << destinations << " \0";

    uint8_t array[8];
    addSize(array, ss.str().size() + 8);

    std::stringstream sd;
    writeToSS(array, sizeof(array), sd);
    sd << ss.str();

    const uint8_t* msg = (uint8_t*)sd.str().c_str();
    sendto(sock, msg, sd.str().size(), 0, (sockaddr*)&ServerAddress, sizeof(ServerAddress));

}

void payloadHeader(std::stringstream& ss, std::stringstream& sd) {
    uint8_t array[8];
    addSize(array, ss.str().size() + 8);

    writeToSS(array, sizeof(array), sd);
    sd << ss.str();

}

void msgHeader(std::stringstream& sf, std::stringstream& sd, uint8_t op) {
    std::stringstream ss;
    ss << "2 " << UI.uid << op;

    ss << sf.str();
    payloadHeader(ss, sd);

}


void sendToClients(uint8_t* msg, uint64_t size, uint8_t op) {
    std::stringstream ss;
    std::stringstream sf;
    std::stringstream sd;

    writeToSS(msg, size, ss);
    payloadHeader(ss, sf);

    msgHeader(sf, sd, op);

    const uint8_t* data = (uint8_t*)sd.str().c_str();
    //printMsg(data, sd.str().size());
    printf("Message Size: %d\n", sd.str().size());
    sendto(sock, data, sd.str().size(), 0, (sockaddr*)&ServerAddress, sizeof(ServerAddress));
}

void recvUID() {
    uint8_t uid;
    recvfrom(sock, &uid, sizeof(uint8_t), 0, NULL, NULL);

    UI.uid = uid;
    printf("Recieved UID: %d\n", UI.uid);
}

void joinServer() {
    uint8_t data[30] = "";
    copyName(data, UI.name, 10);
    data[8] = '0';
    data[9] = ' ';

    addSize(data, sizeof(data));
    sendto(sock, data, sizeof(data), 0, (sockaddr*)&ServerAddress, sizeof(ServerAddress));
    printMsg(data, sizeof(data));

    recvUID();
}

std::vector<uint8_t> recvFromClient() {

    socklen_t addr_size;

    uint8_t array[8];
    recvfrom(sock, array, sizeof(uint8_t)*8, MSG_PEEK, (sockaddr*)&ServerAddress, &addr_size);
    uint64_t size = getSize(array);

    printf("Size: %" PRIu64 "\n", size);

    uint8_t data[size];
    recvfrom(sock, data, size, MSG_WAITALL, (sockaddr*)&ServerAddress, &addr_size);
    //printMsg(data, size);

    return std::vector<uint8_t>(data + 8, (data + 8) + (sizeof(data) - 8) / sizeof data[0]);
}
