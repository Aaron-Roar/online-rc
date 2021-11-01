#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>

int encode_scale = 50;
std::vector<int> jpeg = {cv::IMWRITE_JPEG_QUALITY, encode_scale};
std::vector<int> png = {cv::IMWRITE_PNG_COMPRESSION, encode_scale};
std::vector<int> encodings[] = {jpeg, png};
std::string extensions[] = {".jpg", ".png"};


std::vector<unsigned char> encodeFrame(cv::Mat new_frame, int encode_style) {

    std::vector<unsigned char> buf;

    cv::imencode(extensions[encode_style], new_frame, buf, encodings[encode_style]);

        return buf;
}

cv::Mat decodeFrame(std::vector<unsigned char> buf) {

    unsigned char* array = &buf[0];

    cv::Mat rawData(1, buf.size(), CV_8UC1, array);
    cv::Mat decoded_frame = imdecode(rawData, cv::IMREAD_COLOR);

    return decoded_frame;
}
