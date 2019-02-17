#ifndef REMOTECONTROL_EXAMPLEPAYLOAD_H
#define REMOTECONTROL_EXAMPLEPAYLOAD_H

#include <AbstractPayload.h>

class ExamplePayload : public AbstractPayload {
   private:
    const uint8_t headerSize = 1;
    const uint8_t payloadSize = 1;
    const uint8_t type = PayloadType::EXAMPLE_PAYLOAD;

   public:
    ExamplePayload();
    ~ExamplePayload();
    bool execute(uint8_t* packetPayload);
    bool build(Packet* packet);
};

ExamplePayload::ExamplePayload() {
}

ExamplePayload::~ExamplePayload() {
}

bool ExamplePayload::execute(uint8_t* packetPayload) {
    return true;
}

bool ExamplePayload::build(Packet* packet) {
    packet->setPacketType(this->type);
    packet->createHeaderBuffer(this->headerSize);
    packet->createPayloadBuffer(this->payloadSize);
    return true;
}

#endif