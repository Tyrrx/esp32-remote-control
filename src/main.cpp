#include <Arduino.h>
#include <Remote.h>

uint8_t key[16] = {0xfe, 0xaa, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c,
                   0x6d, 0xaa, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08};

Remote remote(key, 16);

void setup() {
    Serial.begin(9600);
    /*
    Serial.println("start");
    ExamplePayload* payload = new ExamplePayload();
    payload->setPayload(222);
    Packet* tosendpacket = remote.packetBuilder->encode(payload);
    for (size_t i = 0; i < tosendpacket->getBufferSize(); i++) {
        Serial.print(tosendpacket->getBuffer()[i]);
        Serial.print(",");
    }
    Serial.println();

    Packet* packet = remote.packetBuilder->create(tosendpacket->getBufferSize());
    memcpy(packet->getBuffer(), tosendpacket->getBuffer(), tosendpacket->getBufferSize() * sizeof(uint8_t));
    if (remote.packetBuilder->decode(packet)) {
        AbstractPayload* payload = remote.registry->getFactory(packet->getPacketType())->create(packet->getHeader());
        payload->execute(packet->getPayload());
        delete payload;
    }
    delete packet;
    */
}

void loop() {
    remote.receive();
    Serial.println(system_get_free_heap_size());
}

/*
void loop() {
      ExamplePayload* payload = new ExamplePayload();
    payload->setPayload(222);
    remote.send(payload);
    delete payload;
    Serial.println(system_get_free_heap_size());
    delay(60);
}
*/
