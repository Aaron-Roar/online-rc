#include <vector>
#include <string.h>
#include <iostream>
#include <cstdlib>

#include "../../socketLibrary.cpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int main() {
    uint8_t array[20] = "Ricky";
    copyName(UI.name, array, 0);

    setServerAddress("0.0.0.0", 8000);
    joinServer();

    while(true) {
        std::vector<uint8_t> vec = recvFromClient();
        cv::Mat frame = recvFrame(vec, vec.size());

        cv::imshow("View", frame);
        cv::waitKey(1);
        printf("Recieved frame\n");
    }
}


