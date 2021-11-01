#include <vector>
#include <string.h>
#include <iostream>
#include <cstdlib>

//#include "node.cpp"
#include "../../server.cpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/*
void addToHeap(char* arry, char value, int max) {
    int i = 0;

    while(i < max) {
        arry[i] = value;
        i+=1;
    }
}
 void printHeap(char* arry, int max) {
     int i = 0;

     printf("First address is: %d\n", &arry[0]);
     while(i < max) {
         printf("%c", arry[i]);
         i+=1;
     }
     printf("\n");
 }

int main() {
    char* array = (char*)calloc(1, sizeof(char[5]));
    addToHeap(array, 'A', 5);
    printHeap(array, 5);

    char* array2 = (char*)calloc(1, sizeof(char[5]));
    addToHeap(array2, 'B', 5);
    printHeap(array2, 5);

    array = (char*)realloc(array, sizeof(char[20]));
    addToHeap(array, 'C', 20);
    printHeap(array, 20);

    printHeap(array2, 5);


//  int k = 0;
//   struct Node node;
//
//   char arry[30] = "\0 Robot\0 0.0.0.0\0 8005";
//   node.addToTable(arry);
//
//   printf("Table fn: %s\n", Backend::tableIp(0));
//
//   struct sockaddr_in address = Backend::tableToAddress(0);
//
//   char identity[30] = "\0 ";
//   char name[10] = "Ricky";
//
//   Backend::addressToChars(identity, name, address);
//   printf("Full identity:\n%s\n%s\n%s\n", identity + 2, identity + 9, identity + 18);
}
*/

socklen_t len;
void addClients() {
    uint8_t name[] = "0 Bob";
    sockaddr_in Address;
    Address.sin_family = AF_INET;
    Address.sin_port = htons(8000);
    Address.sin_addr.s_addr = INADDR_ANY;

    uint8_t name2[] = "0 Ricky";
    sockaddr_in Address2;
    Address2.sin_family = AF_INET;
    Address2.sin_port = htons(8001);
    Address2.sin_addr.s_addr = INADDR_ANY;

    uint8_t name3[] = "0 Albert";
    sockaddr_in Address3;
    Address3.sin_family = AF_INET;
    Address3.sin_port = htons(8002);
    Address3.sin_addr.s_addr = INADDR_ANY;

    chooseDirection(name, Address, len);
    chooseDirection(name2, Address2, len);
    chooseDirection(name3, Address3, len);

    printAllNodes();
    u_int8_t protocol[29] = "+ Bob:1 Ricky Albert \0\0\0";
    //mutateDestination(protocol, Address);


    //printNamesInDests(searchNodes(name), '1');
    printf("Program finished\n");
}

int main() {
    addClients();
    printf("Bob: %s\n", Nodes[0].name);
    printf("Ricky: %s\n", Nodes[1].name);
    printf("Albert: %s\n", Nodes[2].name);
}
