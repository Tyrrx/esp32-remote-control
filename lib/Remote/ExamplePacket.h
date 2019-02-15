#ifndef REMOTECONTROL_EXAMPLEPACKET_H
#define REMOTECONTROL_EXAMPLEPACKET_H

#include <Packet.h>

class ExamplePacket : Packet {
   public:
    ExamplePacket(AuthenticatedCipher *cipher, uint8_t bufferSize);
    ~ExamplePacket();

    bool build();
    bool parse();

    uint8_t getPayloadExample();
    uint8_t getHeaderExample();
    void setPayloadExample();
    void setHeaderExample();

   private:
    uint8_t payloadExample;
    uint8_t headerExample;
};

ExamplePacket::ExamplePacket(AuthenticatedCipher *cipher, uint8_t bufferSize) : Packet(cipher, bufferSize) {
    this->setPacketType(0);
    this->setPayloadSize(1);
    this->setHeaderSize(1);
}

ExamplePacket::~ExamplePacket() {
}

bool ExamplePacket::build() {
    this->header[0] = this->headerExample;
    this->payload[0] = this->payloadExample;
}

bool ExamplePacket::parse() {
    this->headerExample = this->header[0];
    this->payloadExample = this->payload[0];
}
#endif