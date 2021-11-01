#include "../../../RoboCMD/RoboCMD.h"

int main() {
    uint8_t array[20] = "Jeff";
    copyName(UI.name, array, 0);

    setServerAddress("0.0.0.0", 8000);
    joinServer();

    RoboCMD <float, float, float, float> cmd;

    int i = 0;
    while(true) {
        printf("\nMessage: %d\n", i);
    cmd.setMsg(recvFromClient());
    cmd.disMsg();
    printf("\n\n");
    i += 1;
    }

}


