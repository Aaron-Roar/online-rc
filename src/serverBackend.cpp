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

#include "protocol.h"
#include "serverTypes.h"

//on Msg recive
struct Service {
    private:
        Hash::Table<Server::Client, char> clients;

        uint8_t uidIndex = 0;

        // Should it be here?
        //char name[20] = "\0";

        bool set_address;
        struct sockaddr_in server_address;
        socklen_t server_address_length;
        int sockfd;

        struct epoll_event epollInit(int fd) {
            struct epoll_event event;
            event.events = EPOLLIN | EPOLLEXCLUSIVE;
            event.data.fd = fd;

            return event;
        }

        void gameLoop() {
            struct sockaddr_in Client;
            socklen_t len;

            struct epoll_event events[1];

            events[0] = epollInit(sockfd);
            int epfd = epoll_create1(0);
            epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &events[0]);

            //in future compare and exchange with seperate threads 
            //recieving from linked list que

            Protocol::ServerMsg msg;

            while(true) {
                epoll_wait(epfd, events, 1, -1);
                recvfrom(events[0].data.fd, &msg, sizeof(msg), MSG_PEEK | MSG_DONTWAIT, (struct sockaddr*)&Client, &len);
                uint8_t allBytes[msg.size];
                recvfrom(events[0].data.fd, allBytes, msg.size, MSG_WAITALL | MSG_DONTWAIT, NULL, NULL);

                char ip[30];
                char port[10];
                getnameinfo((struct sockaddr*)&Client, len, ip, sizeof(ip), port, sizeof(port), NI_NUMERICSERV);
                printf("Client address\nIp: %s\nPort: %s\n\n", ip, port);
                payloadDetection(allBytes, Client, len);

                printClientInfo();
            }
        }

        void payloadDetection(uint8_t* allBytes, sockaddr_in address, socklen_t len) {
            uint8_t offset = sizeof(Protocol::ServerMsg) + 1;

            switch(((Protocol::ServerMsg*)allBytes)->instruction) {
                case 0:
                    //
                    break;
                case 1:
                    //Join
                    Resources::testOutput("Protocol::Join\n");
                    {
                        printf("Clients name: %s\n", ((Protocol::Join*)(allBytes + offset))->name);
                    Server::Client new_client = createClient((Protocol::Join*)(allBytes + offset), address, len);
                    Protocol::JoinAck ack;
                    ack.Head.type = Protocol::Join_Ack;
                    printf("Clients name: %s\n", new_client.name);
                    if(addClient(new_client) == 0) {
                        ack.uid = 0;
                        sendAck<Protocol::JoinAck>(ack, address);
                    } else {
                        ack.uid = new_client.uid;
                        sendAck<Protocol::JoinAck>(ack, address);
                    }
                    }
                    break;
                case 2:
                    //Leave
                    Resources::testOutput("Protocol::Leave\n");
                    removeClient(((Protocol::Leave*)(allBytes + offset))->uid);
                    break;
                case 3:
                    //SendToAClient
                    break;
                case 4:
                    //MutateDest
                    Resources::testOutput("Protocol::MutateDests\n");
                    mutateDest((Protocol::MutateDest*)(allBytes + offset));
                    break;
                case 5:
                    break;
                default:
                    break;
            }
        }

        template<typename Ack> void sendAck(Ack ack, sockaddr_in address) {
            sendto(sockfd, &ack, sizeof(Ack), 0, (sockaddr*)&address, sizeof(address));
        }

        uint8_t generateUid() {
            return (uidIndex += 1);
        }

        bool authenticate() {
            return false;
        };

        void serverInstruction() {
        };

        //server instructions
        void sendToDests() {
        };

        Server::Client createClient(Protocol::Join* msg, sockaddr_in address, socklen_t len) {
            Server::Client client;

            Resources::stringCopy(client.name, msg->name);
            client.uid = generateUid();
            client.temperature = 100;
            client.Address = address;
            client.len = len;

            Resources::testOutput("Created new client\n");
            printf("Client: %s Joined the server\n", client.name);

            return client;

        }

        int addClient(Server::Client client) {
            Resources::testOutput("Added a new client\n");
            if(clients.addElement(client, client.uid) == 0) {
                return 0;
            } else {
                return 1;
            }
        };

        void removeClient(uint8_t uid) {
            Resources::testOutput("Removed a client\n");
            clients.removeElement(uid);
        }

        void mutateDest(Protocol::MutateDest* md) {
            Server::Client* client = clients.getElement(md->uid_to_mutate);
            if(client == 0) {
                Resources::testOutput("Failed to find client of which was mean to mutate dests of. Not mutating dests\n");
                return;
            }

            Hash::Table<Server::Client*, char>* dest_list = (client->DestTable).getElement(md->op);
            if(dest_list == 0) {
                Resources::testOutput("Failed to find the dest_list of specific OP for client of which to mutate dests. Not mutating dests\n");
                return;
            }

            if(md->sign == '+') {
                addDest(dest_list, md->uids);
            }
            else if(md->sign == '-') {
                removeDest(dest_list, md->uids);
            }
            else {
                Resources::testOutput("Failed to parse protocol of Mutate Dests. Sign field required and was not <-> or <+>. Not mutating dests\n");
                return;
            }

        };

        void addDestList() {
        };

        void addDest(Hash::Table<Server::Client*, char>* dest_list, uint8_t* uids) {

            int i = 0;
            while(uids[i] != '\0') {
                Server::Client* client_ptr = clients.getElement(uids[i]);

                if(client_ptr == 0) {
                    Resources::testOutput("Failed to add dest as client to add was not found\n");
                    i += 1;
                    continue;
                }

                dest_list->addElement(client_ptr, uids[i]);
                i += 1;
            }
        };

        void removeDest(Hash::Table<Server::Client*, char>* dest_list, uint8_t* uids) {
            return;
        };

    public:
        void setAddress(struct sockaddr_in address) {
            if(set_address == true) {
                Resources::testOutput("Tried to set an already set address\n");
                return;
            }
            server_address = address;
            sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address));

            set_address = true;
        }

        void setName(char* name_to_assign) {
            //copy name_to_assign to name
        }

        void kill() {
            Resources::testOutput("Requested to kill service\n");
            //atempt to shut down and unallocate things
            Resources::testOutput("Finished ShutDown task\n");
        }

        void start() {
            if(set_address == false /*|| name[0] == '\0'*/) {
                Resources::testOutput("Failed to start service as name or address has not been set\n");
                exit(1);
            }

            gameLoop();
        }


        //debug
        void printClientInfo() {
            int i = 0;
            while(i <= uidIndex) {
                Server::Client* client = clients.getElement(i);
                if(client == 0){

                } else {
                printf("Value in table is %s\n", client->name);
                printf("UID in table %d\n", client->uid);

                }
                i += 1;
            }
        };
};
