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

//#include "frame_computation.cpp"
//Protocol
struct MsgHeader {
    int type;
    MsgHeader* next;
};

struct MsgImg {
    struct MsgHeader Head;
    // cv::Mat frame;
};

struct MsgCon {
    struct MsgHeader Head;
    std::vector<uint8_t> coms;
};

struct MutateDest {
    uint8_t nameToMutate[20];
    uint8_t sign;
    uint8_t op;

    uint8_t* names; //Names seperated by a space
    uint8_t* uids; //uids seperated by a space
};

struct Join {
    uint8_t name[20];
    uint8_t bio[20];
};

struct Leave {
    uint8_t uid;
    uint8_t name[20];
};

struct ServerMsg {
    uint64_t size;
    uint8_t instruction;
    uint8_t uid;

    uint8_t op;
    void* payload;
    //The address after this is the payload
};
//Protocol

//Turn into Linked lists
class LinkedList {
    //RESTRICT THE USER WITH CONSTRUCTOR
    Data* first;
    Data* last;
    private:
            struct Client {
                Client* next;

                uint8_t name[20];
                uint8_t uid;
                int temperature;
                struct sockaddr_in Address;
                socklen_t len;

                LinkedList DestinationDictionary; //Become linked list
            };

            struct NodePtr {
                NodePtr* next;
                struct Client* node;
            };

            struct KeyValue {
                KeyValue* next;
                uint8_t op;
                struct NodePtr* value;
            };
        union Data {
            Client client;
            NodePtr nodePtr;
            KeyValue dict;
        };
        Data node;

    public:
        addClient(uint8_t uid, ) {
            node.client = (struct Client*)malloc(sizeof(struct Client));
            node.client->uid;
        }

};

struct ListWrap {
    uint8_t type;

    void* prev;
    void* current;
    void* next;

};


