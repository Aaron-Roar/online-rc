#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

int encode_scale = 50;
std::vector<int> jpeg = {cv::IMWRITE_JPEG_QUALITY, encode_scale};
std::vector<int> png = {cv::IMWRITE_PNG_COMPRESSION, encode_scale};
std::vector<int> encodings[] = {jpeg, png};
std::string extensions[] = {".jpg", ".png"};


std::vector<uint8_t> encodeFrame(cv::Mat new_frame, int encode_style) {

    std::vector<uint8_t> buf;

    cv::imencode(extensions[encode_style], new_frame, buf, encodings[encode_style]);

    return buf;
}

cv::Mat decodeFrame(std::vector<uint8_t> buf) {

    uint8_t* array = &buf[0];

    cv::Mat rawData(1, buf.size(), CV_8UC1, array);
    cv::Mat decoded_frame = imdecode(rawData, cv::IMREAD_COLOR);

    return decoded_frame;
}

std::vector<uint8_t> deserializeFrame(uint8_t cstr[], int size) {
    std::stringstream ss;

    int count = 0;
    while(count < size) {
        ss << cstr[count];
        count += 1;
    }
    std::vector<uint8_t> vec;
    {
        cereal::BinaryInputArchive archive(ss);
        archive(CEREAL_NVP(vec));
    }
    ss.str("");

    return vec;
}
