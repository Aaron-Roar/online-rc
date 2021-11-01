#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>

namespace Resources {
    int Test;
    void testOutput(const char* value) {
        if(Test == 1)
            printf("[#]%s", value);
    }

    void testOutput(int value) {
        if(Test == 1)
            printf("%d\n", value);
    }

    template<typename T> void check(T val1, T val2) {
        if(Test == 1) {
            if(val1 != val2) {
                testOutput("Check on Test FAILED!!!\n");
                printf("Value1: %ld, Value2: %ld\n", val1, val2);
                exit(1);
            } else {
                testOutput("Check on Test PASSED\n");
            }
        }
    }

    int stringCompare(uint8_t* str1, uint8_t* str2) {
        int i = 0;
        while(true) {
            if(str1[i] != str2[i]) {
                return -1;
            }
            else if(str1[i] == '\0') {
                return 0;
            }
            else{
                i += 1;
            }
        }
    }

    int stringCopy(uint8_t* dest, uint8_t* src) {
        int index = 0;

        while(src[index] != '\0') {
            dest[index] = src[index];
            index += 1;
        }
        return index;
    }

    void testStrCopy() {
        uint8_t src_data[20] = "Hello World";
        uint8_t dest_data[20];
        int bytes = stringCopy(dest_data, src_data);
        if(stringCompare(src_data, dest_data) == 0) {
            testOutput("Test StringCopy passed\n");
        } else {
            testOutput("Test StringCopy failed\n");
            exit(1);
        }
    }

    void testStrCmp() {
        uint8_t str1[20] = "Hello World";
        uint8_t str2[20] = "Hello World";
        int result = stringCompare(str1, str2);
        if(result == 0) {
            testOutput("Test StringCompare passed\n");
        }
        else{
            testOutput("Test StringCompare failed\n");
            exit(1);
        }

    }

    void testAll() {
        testStrCmp();
        testStrCopy();
    }
};
