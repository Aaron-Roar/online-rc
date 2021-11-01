//Havent tested as need to VNC to see frames shown on screen
//
//Aarons currently writing this but feel free to contribute :)

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui.hpp>

#include "../../socketLibrary.cpp"

cv::Mat retrieveFrame(cv::VideoCapture vidCap) {
    cv::Mat frame;

    if ((vidCap).isOpened())
    {
        //vid.set(cv::CAP_PROP_FRAME_HEIGHT, 900);
        //vid.set(cv::CAP_PROP_FRAME_WEIGHT, 1440);
        (vidCap).read(frame);
        //cv::imshow("view", frame);
        cv::waitKey(2);
        printf("read frame\n");
        return frame;
    }

    perror("VideoCapture is not opened, cost connection with camera\n");
    exit(699);
}




int main() {
uint8_t array[20] = "RobotCamera";
copyName(UI.name, array, 0);

setServerAddress("0.0.0.0", 8000);
joinServer();

uint8_t name[] = "RobotCamera";
uint8_t dests[] = "Ricky Jeff";
mutateDestinations(name, '+', '1', dests);

    cv::VideoCapture vidCap = cv::VideoCapture(0, cv::CAP_ANY); //0 is default, but 1 2 ... (inf) can be other cams
    cv::Mat frame;

    while(vidCap.isOpened()) {
        frame = retrieveFrame(vidCap);
        std::string str = serealizeFrame(frame, encodeFrame(frame, 0));
        uint8_t* arry = (uint8_t*)str.c_str();

        sendToClients(arry, str.length(), '1');
    }

    //vidCap.~VideoCapture();
}
