#include <opencv2/highgui/highgui.hpp>

#include "socketLibrary.cpp"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <string>
#include <vector>

//vector<char> int size -> IO
void output(std::vector<char> data, int size) {

    int i = 0;
    while(i < size) {
        std::cout<<data[i]<<'\n';
        i+=1;
    }
}

struct sockaddr_in serveraddr;
int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

int main() {
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8000);
    serveraddr.sin_addr.s_addr = inet_addr("0.0.0.0");

    struct FrameStructure Overhead = recvFrameOverhead(sockfd, serveraddr);
    int rows = Overhead.rows;
    int cols = Overhead.cols;

    cv::Mat current_frame = cv::Mat(rows, cols, CV_8UC4);
    while(true) {

        current_frame = recvFrame(sockfd, serveraddr);

        //cv::cvtColor(current_frame, current_frame, cv::COLOR_BGR2RGBA);
        std::cout<<"trying to show \n";
        cv::imshow("view", current_frame); 
        std::cout<<"showed image\n";

        // Display frame for N milliseconds
        cv::waitKey(2);
    }
}
