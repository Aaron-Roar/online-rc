#include "protocol.h"
#include "resources.h"

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

struct Client {
        uint8_t uid = 0;
    private:
        uint8_t name[20];
        bool joined = false;
        bool set_address = false;
        int sockfd;
        struct sockaddr_in server_address;
        socklen_t len;

        void sendToServer(uint8_t* data, int size) {
            sendto(sockfd, data, size, 0, (struct sockaddr*)&server_address, sizeof(server_address));
        }

    public:
        void setServerAddress(struct sockaddr_in address) {
            if(set_address == true) {
                Resources::testOutput("The server address has already been set\n");
                return;
            }
            server_address = address;
            sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address));

            set_address = true;
        }

        template<typename T> uint8_t* serialize(uint8_t* array, T data) {
            int bytes = sizeof(data);
            int i = 0;
            while(i < bytes) {
                array[i] = ((uint8_t*)&data)[i];
                i += 1;
            }
            return (array + sizeof(data) + 1);
        }

        void setTest(int state) {
            Resources::Test = state ;
        }

        template<typename T> void buildPayload(uint8_t* data, Protocol::ServerMsg msg, T payload, uint8_t instruction) {
            msg.instruction = instruction;
            msg.uid = uid;

            uint8_t* next_place;

            printf("Name b4 serialization: %s\n", payload.name);
            next_place = serialize(data, msg);
            next_place = serialize(next_place, payload);
            printf("Name after serialization: %s\n", ((Protocol::Join*)(data + sizeof(msg) + 1))->name);
        }

        void leave() {
            if(joined != true) {
                Resources::testOutput("You attempted to leave a server you have not joined\n");
                return;
            }

            Protocol::ServerMsg msg;
            Protocol::Leave payload;

            msg.size = (sizeof(msg) + sizeof(payload));
            uint8_t data[msg.size];

            Resources::stringCopy(payload.name, name);
            payload.uid = uid;

            buildPayload<Protocol::Leave>(data, msg, payload, 2);

            Resources::check<uint64_t>(msg.size, (*(Protocol::ServerMsg*)data).size);

            sendToServer(data, msg.size);
        }

        void join(uint8_t* new_name, uint8_t* extra) {
            if(joined == true) {
                Resources::testOutput("You are tying to join a server you already joined\n");
                return;
            }

            Protocol::ServerMsg msg;
            Protocol::Join payload;
            msg.size = (sizeof(msg) + sizeof(payload));
            uint8_t data[msg.size];

            Resources::stringCopy(name, new_name);
            Resources::stringCopy(payload.name, name);
            Resources::stringCopy(payload.bio, extra);

            buildPayload<Protocol::Join>(data, msg, payload, 1);
            Resources::check<uint64_t>(msg.size, (*(Protocol::ServerMsg*)data).size);
            sendToServer(data, msg.size);
            joined = true;
            printf("Name is %s\n", payload.name);
            printf("Name is also %s\n", ((Protocol::Join*)(data + sizeof(msg) + 1))->name);

            Protocol::JoinAck ack;
            //recvfrom(sockfd, &ack, sizeof(ack), 0, (sockaddr*)&server_address, &len);
            ack.uid = uid;
        }

        void mutateDests(uint8_t uid_to_mutate, uint8_t op, uint8_t* uids, uint8_t sign) {
            if(joined == false) {
                Resources::testOutput("You are attempting to send a message to a server you have not joined\n");
                return;
            }

            Protocol::ServerMsg msg;
            Protocol::MutateDest payload;

            msg.size = (sizeof(msg) + sizeof(payload));
            uint8_t data[msg.size];

            payload.uid_to_mutate = uid_to_mutate;
            Resources::stringCopy(payload.uids, uids);
            payload.sign = sign;
            payload.op = op;
            
            //buildPayload<Protocol::MutateDest>(data, msg, payload, 4);
            Resources::check<uint64_t>(msg.size, (*(Protocol::ServerMsg*)data).size);
            sendToServer(data, msg.size);
        }

};
