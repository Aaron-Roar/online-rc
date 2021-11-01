#include <vector>
#include <string.h>
#include <iostream>
#include <cstdlib>

#include "node.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

std::vector<char*> lookup_table; // name:0.0.0.0:8001


char* nextToken(char* arry, int size, char token) {
    int i = 0;
    while(i < size) {
        if(arry[i] == token) {
            std::cout<<"Next Token: "<<&arry[i+1]<<"\n";
            return &arry[i + 1];
        }
        i+=1;
    }
    perror("Accessing out specified size");
    exit(0);
}

int search_table(char* name) {
    int i = 0;
    while(i < lookup_table.size()) {
        std::cout<<"Searching table\n";
        if(strcmp(name, nextToken(lookup_table[i], 20, '\0')) == 0) {
            std::cout<<"Found name\n";
            return i;
        } 
        i += 1;
    }
    return (-1);
}

//truct sockadd_in tableToAddress(int index) {
//   struct sockaddr_in table_addr;
//
//   table_addr.sin_family = AF_INET;
//   table_addr.sin_port = htons(std::8000);
//   table_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
//

int main() {
    char arry[30] = "\0 Aaron\0 192.168.2.3\0 8003";
    char arry2[30] = "\0 John\0 1.0.0.0\0 8920";
    char arry3[30] = "\0 Mike\0 124.154.3.6\0 8769";

    struct Node node;
    node.addToTable(arry);
    node.addToTable(arry2);
    node.addToTable(arry3);

    char sample_name[20] = "Mike";
    int index = node.search_table(sample_name);

    printf("\n\nSearch result for Mike: \n");
    printf("Name: %s\n", node.tableName(index));
    printf("Ip: %s\n", node.tableIp(index));
    printf("Port: %s\n\n", node.tablePort(index));

    //node.outputTable();

    unsigned char name[16] = "Aaron";
    char ip[30] = "192.168.2.3";
    int port = 8005;

    node.name = name;
    node.setPersonnelAddress(port, ip);
    node.sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    //node.outputName();

    node.outputAddress(node.tableToAddress(1));

}
