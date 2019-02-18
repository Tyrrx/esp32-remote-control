#include <unity.h>

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // // clean stuff up here
}

void smartTest(void) {
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(smartTest);
    UNITY_END();

    return 0;
}
