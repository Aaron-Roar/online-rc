#include <vector>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sstream>

#include "frame_computation.cpp"
#include "server.cpp"

//Recieves an array of data from some other machine connected to the 
//server following the set protocol Which has the size as the first 8 
//bytes
std::vector<uint8_t> recvFromClient();

//Requeets to join the server providing its prefered name and 
//instruction for the server to follow
//If successful recieves it uid back, else recieves 0
void joinServer();


//Recives a UID from the server
void recvUID();


//Sends the message <msg> to the server to be forwarded to other 
//clients. The <size> indicates the size of your desired message and the 
//<op> is what type of message this is
void sendToClients(uint8_t* msg, uint64_t size, uint8_t op);


//Adds the required header information to abide by the protocol which 
//includes the size of the message the <op> meaning the type and adds 
//payload header after
void msgHeader(std::stringstream& sf, std::stringstream& sd, uint8_t op);

//Adds a header onto a message that is specific to the payload, 
//usually just the size
void payloadHeader(std::stringstream& ss, std::stringstream& sd);

//Requests the server to add or remove someone from someones 
//destination list
void mutateDestinations(uint8_t* name, uint8_t sign, uint8_t op, uint8_t* destinations);

//Sets the servers address which will be used to communicate with 
//other clients through
void setServerAddress(const char* ip, int port);


//Writes each element of a contiguous data structure to a string 
//stream
void writeToSS(uint8_t* msg, uint64_t size, std::stringstream& sd);


struct Header {
    int type;
    Header* next;
};

struct MsgImg {
    struct Header Head;
    cv::Mat frame;
};

struct MsgCon {
    struct Header Head;
    std::vector<uint8_t> coms;
};

struct MutateDest {
}

struct Join {

}

struct Leave {

}

struct SendData {

}

union Payload {
    struct MsgCon cons;
    struct MsgImg Img;
};

struct ServerMsg {
    uint64_t size;
    uint8_t instruction;
    union Payload payload;
};



void appendInfo(struct Header* NewHeadInfo);

void append(struct Header* NewHead);

void print(struct Header* CurrentHead);
