//Sample protocol
//
//Sending data
//{Server instruction (byte)} {UID (byte)} {Operation Number (byte)} {Payload (dynamic)}
//
//Initial M=Signup
//{Server instruction (byte)} {Name (dynamic)}

//+(Bob 2){Ricky Tommy John}
//-(Bob 2){Ricky John}
//
//BBut = 0x02
//
//ButtonA : 0x01
//ButtonB : 0x02

#include <inttypes.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sys/epoll.h>
#include <thread>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

//#include "node.cpp"
int Test;
void testOutput(const char* value) {
    if(Test == 1)
    printf("[#]%s", value);
}

void testOutput(int value) {
    if(Test == 1)
        printf("%d\n", value);
}

int node_index = 0;
uint8_t uid_index = 2;
int sockfd;

int thread_index = 0;
std::thread* threads = (std::thread*)calloc(1, 10*sizeof(std::thread));

struct epoll_event epollInit(int fd) {
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLEXCLUSIVE;
    event.data.fd = fd;

    return event;
}

struct Node {
    uint8_t name[20];
    uint8_t uid;
    int temperature;

    struct sockaddr_in Address;
    socklen_t len;

    int dict_index;
    struct Destinations* DestinationDictionary;
};

struct Destinations {
    uint8_t op;
    struct Node** value;
};

// global list of hosts for all
struct Node* Nodes = (struct Node*)calloc(10, sizeof(struct Node));

uint64_t getSize(uint8_t* size_buffer) {
    uint64_t large = 0;
    int i = 0;

    while(i < 8) {
        large |= (uint64_t)size_buffer[i] << (56 - (8*i));
        i += 1;
    }
    printf("Returning converted value\n");

    return large;
}

void copyName(uint8_t* name, uint8_t* nameToCopy, int index) {
    int i = 0;
    while(i < 20) {
        if(nameToCopy[i] == '\0')
            break;

        name[index] = nameToCopy[i];
        i += 1;
        index += 1;
    }
}

void addNode(uint8_t* name, struct sockaddr_in address, socklen_t len) {
    copyName(Nodes[node_index].name, name, 0);
    Nodes[node_index].uid = uid_index;
    Nodes[node_index].Address = address;
    Nodes[node_index].temperature = 100;
    Nodes[node_index].dict_index = 0;
    Nodes[node_index].len = len;

    struct Destinations* DestinationDictionary = (struct Destinations*)calloc(1, 10*sizeof(struct Destinations));
    Nodes[node_index].DestinationDictionary = DestinationDictionary;

    node_index += 1;
    uid_index += 1;
}

struct Node** searchDict(struct Node* Client, uint8_t op) {
    int counter = 0;
    while(counter < Client->dict_index) {
        if(Client->DestinationDictionary[counter].op == op) {
            printf("found destination in dict\n");
            return Client->DestinationDictionary[counter].value;
        }
        counter += 1;
    }
    printf("Failed to find dict!!!!\n");
    return 0;
}

void addToDests(struct Node* Client, struct Node* ClientToAdd, uint8_t op) {
    struct Node** Destinations = searchDict(Client, op);
    int i = 0;

    while(true) {
        if(Destinations[i] == 0) {
            Destinations[i] = ClientToAdd;
            return;
            printf("John Cena");
        }
        i += 1;
    }
}

void addToDict(struct Node* Client, uint8_t op) {
    struct Node** Destinations = (struct Node**)calloc(1, 10*sizeof(struct Node*));

    Client->DestinationDictionary[Client->dict_index].value = Destinations;
    Client->DestinationDictionary[Client->dict_index].op = op;

    Client->dict_index += 1;
}

struct Node* searchNodes(uint8_t uid) {
    int i = 0;
    while(i < node_index) {
        if(Nodes[i].uid == 0) {
            return 0;
        }
        else if(Nodes[i].uid == 1) {
            continue;
        }
        else if(uid == Nodes[i].uid) {
            return &Nodes[i];
        }
        i += 1;
    }
    return 0;
}

int stringCompare(uint8_t* str1, uint8_t* str2) {
    int i = 0;
    while(true) {
        if(str1[i] != str2[i]) {
            return -1;
        }
        else if(str1[i] == '\0') {
            return 0;
        }
        else{
            i += 1;
        }
    }
}

struct Node* searchNodes(uint8_t* name) {
    int i = 0;
    while(i < node_index) {
        if(stringCompare(name, Nodes[i].name) == 0) {
            printf("In searchNodes compare result: 0, names compared were %s and %s\n", Nodes[i].name, name);
            return &Nodes[i];
        }
        //std::cout<<sName<<" and "<<reinterpret_cast<char*>(name)<<" are not equal\n";
        i += 1;
    }
    printf("Failed to find node\n");
    return 0;
    //fail
}

struct Node* authenticateNode(char uid, sockaddr_in Address) {
    struct Node* Client = searchNodes(uid);
    if(Client == 0) {
        perror("Client had wrong uid: REJECTED\n");
        printf("UID: %d or %c\n", uid, uid);
        return 0;
    } 
    if(Client->Address.sin_port != Address.sin_port) {
        perror("Client had wrong port: REJECTED\n");
        return 0;
    }
    if(Client->Address.sin_addr.s_addr != Address.sin_addr.s_addr) {
        perror("Client had wrong ip: REJECTED\n");
        return 0;
    }
    else {
        return Client;
    }
}

void initialConnect(uint8_t* msg, sockaddr_in Address, socklen_t len) {
    if(searchNodes(msg) != 0) {
        //check if other stuff is in use
        printf("Client wanted to connect with name real:%s, but %s already exits\nSending failure code\n\n", searchNodes(msg)->name, msg);
        char fail[1] = {'0'};
        sendto(sockfd, fail, sizeof(char), 0, (struct sockaddr*)&Address, sizeof(Address));
    }
    else {
        addNode(msg, Address, len);
        printf("Added node with name: %s\n", msg);
        uint8_t* uid = &searchNodes(msg)->uid;

        sendto(sockfd, &uid[0], sizeof(uint8_t), 0, (struct sockaddr*)&Address, len);
        printf("Sent UID: %d\n", *uid);
    }
}

void sendToDestinations(uint8_t* msg, struct Node** Destinations) {
    int j = 0;
    printf("Message server is sending\n");
    while(j < 20) {
        if(msg[j] == '\0') {
            //printf("[NULL]");
        } else{
        printf("%c", msg[j]);
        }
            j += 1;
    }
    printf("\n");

    int i = 0;
    while(true) {
        if(Destinations[i] == 0)
            break;
        sendto(sockfd, msg, getSize(msg), 0, (struct sockaddr*)&Destinations[i]->Address, Destinations[i]->len);
        printf("\n Server sent msg: %s to client: %s\n", msg, Destinations[i]->name);
        printf("First dec of msg: %d\n", msg[0]);
        printf("Size: %" PRIu64 "\n", getSize(msg));
        i += 1;
    }
}

void passData(uint8_t* msg, sockaddr_in Address) {
    int j = 0;
    printf("Message server is sending\n");
    while(msg[j] != '\0') {
        printf("%c", msg[j]);
            j += 1;
    }
    printf("\n");
    struct Node* Client = authenticateNode(msg[0], Address);
    if(Client == 0) {
        //notify client he was not permitted
        exit(0);
    } else {
        printf("Client AUthenticated with uid %d\n", msg[0]);
        sendToDestinations(msg + 2, searchDict(Client, msg[1]));
    }
}

int sizeOfName(uint8_t* msg) {
    int size = 0;
    while(true) {
        if(msg[size] == ' ' || msg[size] == ':' || msg[size] == '-' || msg[size] == '+') {
            break;
        }
        size += 1;
    }
    return size;
}

void populateArray(uint8_t* msg, uint8_t* array, int size) {
    int i = 0;
    while(i < size) {
        array[i] = msg[i];
        i += 1;
    }
    array[size] = '\0';
}

Node* readNode(uint8_t** msg) {
    int size = sizeOfName(msg[0]);

    uint8_t array[size + 1];
    populateArray(msg[0], array, size);
    msg[0] += size + 1;

    return searchNodes(array);
}

char readVal(uint8_t** msg) {
    char sign = msg[0][0];
    msg[0] += 2;
    return sign;
}

//internal use only
void printNamesInDests(struct Node* Client, uint8_t op) {
    struct Node** Destinations = searchDict(Client, op);
    int i = 0;

    printf("Names in %s's destination list of %c op\n", Client->name, op);
    while(true) {
        if(Destinations[i] == 0)
            break;
        printf("%s\n", Destinations[i]->name);
        i += 1;
    }
}

void mutateDestination(uint8_t* val, struct sockaddr_in Client) {
    uint8_t* msg = val;

    printf("Protocol: %s\n", msg);
    char sign = readVal(&msg);
    printf("Sign: %c\n", sign);
    printf("Protocol: %s\n", msg);
    Node* MutatingNode = readNode(&msg);
    printf("Protocol: %s\n", msg);
    char op = readVal(&msg);
    printf("Protocol: %s\n\n", msg);

    printf("Pareed protocol header\n\n");
    Node** Destinations = searchDict(MutatingNode, op);
    if(Destinations == 0) {
        addToDict(MutatingNode, op);
        printf("Destination did not exit created destination for %s of %c op\n", MutatingNode->name, op);
    }
    printf("Looked for nodes destinations\n\n");

    Node* NodeInDest;
    while(true) {
        if(msg[0] == '\0')
            break;
        NodeInDest = readNode(&msg);
        printf("Protocol: %s\n", msg);
        if(sign == '+') {
            addToDests(MutatingNode, NodeInDest, op);
        } else {
        }
        printNamesInDests(MutatingNode, op);
    }
}

//internal function
void printAllNodes() {
    int k = 0;
    printf("All nodes on server:\n");
    while(k < node_index) {
        printf("%s\n", Nodes[k].name);
            k += 1;
    }
}

void chooseDirection(uint8_t* msg, sockaddr_in Address, socklen_t len) {
    switch(msg[0]) {
        case '0':
            printf("Directing: initialConnect\n");
            initialConnect(msg + 2, Address, len);
            printf("Finished clients request\n\n");
            printAllNodes();
            break;
        case '1':
            //querry
            break;
        case '2':
            printf("Directing: passData\n");
            passData(msg + 2, Address);
            printf("Finished clients request\n\n");
            printAllNodes();
            break;
        case '3':
            printf("Directing: mutateDestinations\n");
            mutateDestination(msg + 2, Address);
            printf("Finished clients request\n\n");
            printAllNodes();
            break;
    }
}



void serverThread() {
    //epoll stuff
    struct sockaddr_in Client;
    struct epoll_event events[1];
    socklen_t len;

    events[0] = epollInit(sockfd);
    int epfd = epoll_create1(0);
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &events[0]);

    uint8_t array[8];

    while(true) {

        epoll_wait(epfd, events, 1, -1);
        recvfrom(events[0].data.fd, array, sizeof(uint8_t)*8, MSG_PEEK | MSG_DONTWAIT, NULL, NULL);
        uint64_t size = getSize(array);

        printf("Size: %" PRIu64 "\n", size);

        uint8_t data[size];
        recvfrom(events[0].data.fd, data, size, MSG_DONTWAIT | MSG_WAITALL, (struct sockaddr*)&Client, &len);

        char ip[30];
        char port[10];
        getnameinfo((struct sockaddr*)&Client, len, ip, sizeof(ip), port, sizeof(port), NI_NUMERICSERV);
        printf("Client address %s:%s\n", ip, port);
        printf("\nEntire payload\n> ");
        int j = 0;
        while(j < size) {
            printf("%d", data[j]);
            j += 1;
        }
        printf("\nCalled chooseDirection\n");
        printf("Node index: %d\n", node_index);
        chooseDirection(data + 8, Client, len);
    }

}

/*
void spawnThread() {
    std::thread thread(serverThread);
    thread.detach();
}
*/

void addSize(uint8_t* array, uint64_t size) {
    int i = 7;

    while(i > -1) {
        array[i] = (uint8_t)(size >> (7 - i)*(8));
        i -= 1;
    }
}
