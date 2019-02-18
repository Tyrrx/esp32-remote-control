#ifndef REMOTECONTROL_ABSTRACTPAYLOAD_H
#define REMOTECONTROL_ABSTRACTPAYLOAD_H

#include <Packet.h>
#include <PayloadType.h>

class AbstractPayload {
   private:
    /* data */
   public:
    AbstractPayload();
    virtual ~AbstractPayload();

    // Virtual interface to execute the AbstractPayload with given data.
    virtual bool execute(uint8_t* packetPayload);

    //Virtual interface to build a given Packet data from an AbstractPayload.
    virtual bool build(Packet* packet);
};

AbstractPayload::AbstractPayload() {
}

AbstractPayload::~AbstractPayload() {
}

#endif