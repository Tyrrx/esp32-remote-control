#include <Arduino.h>
#include <Packet.h>
#include <unity.h>

Packet *packet;

void setUp(void) {
    packet = new Packet();
}

void tearDown(void) {
}

void test_function_packet_createBuffer(void) {
    uint8_t size = 255;
    packet->createBuffer(size);
    TEST_ASSERT_EQUAL_UINT8(size, packet->getBufferSize());
    TEST_ASSERT_NOT_NULL(packet->getBuffer());
}

void test_function_packet_hasPacketHeaderBuffer(void) {
    TEST_ASSERT_EQUAL(3, packet->getPacketHeaderSize());
    TEST_ASSERT_NOT_NULL(packet->getPacketHeader());
}

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(test_function_packet_createBuffer);
    RUN_TEST(test_function_packet_hasPacketHeaderBuffer);
    UNITY_END();
}

void loop() {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
}