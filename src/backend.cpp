#include <sys/types.h>

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <thread>

#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>


socklen_t sock_length;

namespace Backend {

std::vector<char*> lookup_table;
int sockfd;
const unsigned char* name;
struct sockaddr_in server_addr;

    char* nextToken(char* arry, int size, char token, int tokens) {
        int i = 0;
        int j = 0;
        while(i < size) {
            if(arry[i] == token) {
                j+=1;
                if(j == tokens) {
                    return &arry[i + 2];
                }
            }
            i+=1;
        }
        perror("Accessing out specified size");
        exit(0);
    }

    sockaddr_in tableToAddress(int index){
        std::string port_str(nextToken(lookup_table[index], 30, '\0', 3), nextToken(lookup_table[index], 30, '\0', 3) + 5);

        //printf("Port set %d\n", std::stoi(port_str));
        sockaddr_in Address;
        Address.sin_family = AF_INET;
        Address.sin_port = htons(std::stoi(port_str));
        Address.sin_addr.s_addr = inet_addr(nextToken(lookup_table[index], 30, '\0', 2));

        return Address;
    }

    void appendValue(char* arry, char* value, int field) {
        char* identity = nextToken(arry, 30, '\0', field);

        int i = 0;
        while(true) {
            if(value[i] == '\0') {
                identity[i] = '\0';
                identity[i+1] = ' ';
                printf("Set: %s\n", identity);
                break;
            }

            identity[i] = value[i];
            i+=1;
        }
    }

   void addressToChars(char* identity, char* name, struct sockaddr_in address) {
        char ip[30];
        char port[10];

       int rc = getnameinfo((struct sockaddr*)&address, sizeof(address), ip, sizeof(ip), port, sizeof(port), NI_NUMERICSERV);//, NI_NUMERICHOST | NI_NUMERICSERV);

        appendValue(identity, name, 1);
        appendValue(identity, ip, 2);
        appendValue(identity, port, 3);

    }


    char* tableName(int index) {
        return nextToken(lookup_table[index], 30, '\0', 1);
    }

    char* tableIp(int index) {
        return nextToken(lookup_table[index], 30, '\0', 2);
    }

    char* tablePort(int index) {
        return nextToken(lookup_table[index], 30, '\0', 3);
    }

    int search_table(char* name) {

        int i = 0;
        while(i < lookup_table.size()) {
            std::cout<<"Searching table\n";
            if(strcmp(name, nextToken(lookup_table[i], 30, '\0', 1)) == 0) {
                std::cout<<"Found name\n";
                return i;
            }
            i += 1;
        }

        return (-1);
    }
}
