#include <vector>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

struct Node {
    bool life = false;
    std::vector<std::string> lookup_table;
    bool listening = false;
    int node_count = 0;
    int node_port = 0;

    const int sockfd;
    const int global_port = 0;
    const unsigned char name[16] = {"Default"};
    const unsigned char personnal_address[20] = {"0.0.0.0:8001"};
    const unsigned char data = 0; //byte

    const sockaddr_in device_address;
    const sockaddr_in  server_address;

    void deviceListen() {
        unsigned char initial_message[18]; // @(byte) + name(16 byte) + data(byte)

        //recvfrom(sockfd, &initial_message, sizeof(initial_message), 0, )

    }
};


