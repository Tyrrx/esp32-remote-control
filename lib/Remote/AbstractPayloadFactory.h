#ifndef REMOTECONTROL_ABSTRACTPAYLOADFACTORY_H
#define REMOTECONTROL_ABSTRACTPAYLOADFACTORY_H

#include <AbstractPayload.h>

class AbstractPayloadFactory {
   private:
   public:
    AbstractPayloadFactory();
    ~AbstractPayloadFactory();

    virtual AbstractPayload *create() = 0;
};

AbstractPayloadFactory::AbstractPayloadFactory() {
}

AbstractPayloadFactory::~AbstractPayloadFactory() {
}

#endif