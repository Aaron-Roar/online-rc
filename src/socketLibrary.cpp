#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#include "uiBackend.cpp"

#include <opencv2/highgui/highgui.hpp>

int status;

//Mat -> string
std::string serealizeFrame(cv::Mat new_frame, std::vector<uint8_t> compressed_frame) {
    std::vector<uint8_t> vec;
    if(compressed_frame.size() != 0){
        std::cout<<"Seriliased jpg/png encoding\n";
        vec = compressed_frame;
    } else {
        std::vector<uint8_t> frameVec(new_frame.begin<uint8_t>(), new_frame.end<uint8_t>());
        vec = frameVec;
    }

    std::stringstream ss;
    {
        cereal::BinaryOutputArchive archive(ss);
        archive(CEREAL_NVP(vec));
    }

    return ss.str();

}

//i32 i32 -> u1
struct sockaddr_in sendFrameOverhead(int rows, int cols, struct sockaddr_in serveraddr, struct sockaddr_in clientaddr, int sockfd) {
    socklen_t clientaddrLength, serveraddrLength;

    bool activity = false;
    while(activity == false)
        status = recvfrom(sockfd, &activity, sizeof(bool), 0, (struct sockaddr*)&clientaddr, &clientaddrLength);
    if(status < 0)
        perror("recv error");

    std::cout<<activity<<'\n';

    status = sendto(sockfd, &rows, sizeof(int), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
    if(status < 0)
        perror("semd error");
    status = sendto(sockfd, &cols, sizeof(int), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
    if(status < 0)
        perror("send error");

    std::cout<<"Sent row col stp: "<<rows<<" "<<cols<<'\n';

    return clientaddr;
}


struct FrameStructure {
    int rows = 0;
    int cols = 0;
};

struct FrameStructure recvFrameOverhead(int sockfd, struct sockaddr_in serveraddr) {
    socklen_t clientaddrLength, serveraddrLength;
    struct FrameStructure Overhead;

    bool activity = true;
    status = sendto(sockfd, &activity, sizeof(bool), 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if(status < 0)
        perror("send failed");
    std::cout<<"sent activity\n";

    recvfrom(sockfd, &Overhead.rows, sizeof(int), 0, (struct sockaddr*)&serveraddr, &serveraddrLength);
    recvfrom(sockfd, &Overhead.cols, sizeof(int), 0, (struct sockaddr*)&serveraddr, &serveraddrLength);

    std::cout<<"\nReceived rows cols: "<<Overhead.rows<<" "<<Overhead.cols<<'\n';
    std::cout<<"size of frame "<<Overhead.rows * Overhead.cols<<'\n';

    return Overhead;
}

void sendFrame(cv::Mat new_frame, struct sockaddr_in serveraddr, struct sockaddr_in clientaddr, int sockfd) {
    socklen_t clientaddrLength, serveraddrLength;

    std::vector<uint8_t> encode_vec = encodeFrame(new_frame, 0);
    std::string str = serealizeFrame(new_frame, encode_vec);

    //const char* cstr = str.c_str();

    std::cout<<"size of frame is "<<(int)str.length()<<'\n';
    //status = sendto(sockfd, &size, sizeof(int), 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
    uint8_t* arry = (uint8_t*)str.c_str();
    sendToClients(arry, str.length(), '1');
    if(status < 0) {
        perror("Perameter <size> failed to send..");
        exit(0);
    }
}

cv::Mat recvFrame(std::vector<uint8_t> vec, int size) {
    return decodeFrame(deserializeFrame(&vec[0], vec.size()));
}

