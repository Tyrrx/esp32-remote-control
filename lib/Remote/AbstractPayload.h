#ifndef REMOTECONTROL_ABSTRACTPAYLOAD_H
#define REMOTECONTROL_ABSTRACTPAYLOAD_H

#include <Packet.h>
#include <PayloadType.h>

class AbstractPayload {
   private:
    /* data */
   public:
    AbstractPayload();
    ~AbstractPayload();

    virtual bool execute(uint8_t* packetPayload);
    virtual bool build(Packet* packet);
};

AbstractPayload::AbstractPayload() {
}

AbstractPayload::~AbstractPayload() {
}

#endif