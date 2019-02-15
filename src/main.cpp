#include <Arduino.h>
#include <Remote.h>
uint8_t key[16] = {0xfe, 0xaa, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c,
                   0x6d, 0xaa, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08};

GCM<AES128> *gcm = new GCM<AES128>;
Remote *remote(gcm, key, gcm->keySize());

void setup() {
    Serial.begin(9600);
    remote->registerPacket(ExamplePacket);
}

void loop() {
    if (remote->receive()) {
        //do something
    }
}

void exampleCallback(ExamplePacket *packet) {
    Serial.println(packet->getPayloadExample());
    Serial.println(packet->getHeaderExample());
}