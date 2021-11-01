/** 
 *  @file   node.h
 *  @brief  Backend Comunication Interface header
 *  @author Aaron Rohrer
 *  @date   2021-07-1 
 ***********************************************/

#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

struct Node {

    /** @brief Mutates the |personnel_address| member to use as the 
      nodes address which other nodes will use to comunicate with it.
      @param |int port| is the internal port the node will use to listen on, 
      |const char* ip| is the internal IP the node will lsiten on.
      @return This function returns no value, only mutates the 
      |personnel_address| member.
     **/
    void setPersonnelAddress(unsigned char* name, int port, const char* ip, int transport);

    void deviceAphiliate();

    void connectTo(int index, int instruction);

    void addToTable(char* name_to_add);

    void listen();

    static void streamCamera(int fd, socklen_t sock_l, struct sockaddr_in device, int data);

    //Internal Testing
    //
    void outputTable();

    void outputName();

    void outputAddress(sockaddr_in addr);

};

