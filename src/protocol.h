#include <inttypes.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sys/epoll.h>
#include <thread>

namespace Protocol {
    enum msg_types {Join_Ack, Msg_Img, Msg_Con};

    struct MsgHeader {
        int type;
        MsgHeader* next;
    };

    struct MsgImg {
        struct MsgHeader Head;
        // cv::Mat frame;
    };

    struct MsgCon {
        struct MsgHeader Head;
        std::vector<uint8_t> coms;
    };


    struct MutateDest {
        uint8_t nameToMutate[20];
        uint8_t uid_to_mutate;

        uint8_t sign;
        uint8_t op;

        uint8_t* names; //Names seperated by a space
        uint8_t* uids; //uids seperated by a space
    };

    struct Join {
        uint8_t name[20];
        uint8_t bio[20];
    };

    struct JoinAck {
        struct MsgHeader Head;
        uint8_t uid;
        uint8_t pass;
    };

    struct Leave {
        uint8_t uid;
        uint8_t name[20];
    };

    struct ServerMsg {
        uint64_t size;
        uint8_t instruction;
        uint8_t uid;

        //The address after this is the payload
    };


};
