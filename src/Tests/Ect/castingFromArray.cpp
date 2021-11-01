#include "../../uiBackend.cpp"

struct Data {
    uint32_t value;
    double other_value;
};

template<typename T> void toUChar(uint8_t* dest, T input) {
    int bytes = sizeof(input);
    int i = 0;
    while(i < bytes) {
        dest[i] = ((uint8_t*)&input)[i];
        i += 1;
    }
}

int main() {
//   Client client;
//   client.setTest(1);
//
//   Data data;
//   data.value = 26;
//   data.other_value = 27.0;
//
//   uint8_t array[500];
//   client.serialize<Data>(array, data);
//
//   Resources::check<int>(data.value, (*(Data*)array).value);
    int data = 26;
    uint8_t array[100];

    toUChar(array, data);

    int newData = *(int*)array;
    printf("Data: %d\n", newData);

}

