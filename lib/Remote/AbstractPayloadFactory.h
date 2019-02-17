#ifndef REMOTECONTROL_ABSTRACTPAYLOADFACTORY_H
#define REMOTECONTROL_ABSTRACTPAYLOADFACTORY_H

#include <AbstractPayload.h>

class AbstractPayloadFactory {
   private:
   public:
    AbstractPayloadFactory();
    ~AbstractPayloadFactory();

    virtual AbstractPayload *create(uint8_t *packetHeader);
};

AbstractPayloadFactory::AbstractPayloadFactory() {
}

AbstractPayloadFactory::~AbstractPayloadFactory() {
}

#endif