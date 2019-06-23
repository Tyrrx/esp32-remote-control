#ifndef REMOTECONTROL_CONTROLPAYLOAD_H
#define REMOTECONTROL_CONTROLPAYLOAD_H

#include <AbstractPayload.h>
#include <RemoteService.h>

class ControlPayload : public AbstractPayload {
   private:
    const uint8_t headerSize = 0;
    uint8_t payloadSize = 24;
    const uint8_t type = PayloadType::CONTROL_PAYLOAD;

    RemoteService* remoteService;

   public:
    ControlPayload();
    ControlPayload(RemoteService* remoteService);
    ~ControlPayload();

    // Executes ExamplePayload with given data.
    // Implements AbstractPayload execute().
    bool execute(uint8_t* packetPayload);

    // Builds a given Packets data.
    // Implements AbstractPayload build().
    bool build(Packet* packet);
};

ControlPayload::ControlPayload(/* args */) {}

ControlPayload::ControlPayload(RemoteService* remoteService) { this->remoteService = remoteService; }

ControlPayload::~ControlPayload() {}

bool ControlPayload::execute(uint8_t* packetPayload) {
    this->remoteService->handleReceive(packetPayload);
    return true;
}

bool ControlPayload::build(Packet* packet) {
    packet->setPacketType(this->type);
    packet->createHeaderBuffer(this->headerSize);
    packet->createPayloadBuffer(this->payloadSize);
    return true;
}

#endif