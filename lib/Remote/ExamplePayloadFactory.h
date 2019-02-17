#ifndef REMOTECONTROL_EXAMPLEPAYLOADFACTORY_H
#define REMOTECONTROL_EXAMPLEPAYLOADFACTORY_H

#include <AbstractPayload.h>
#include <AbstractPayloadFactory.h>
#include <ExamplePayload.h>

class ExamplePayloadFactory : public AbstractPayloadFactory {
   private:
   public:
    ExamplePayloadFactory();
    ~ExamplePayloadFactory();
    AbstractPayload* create(uint8_t* packetHeader);
};

ExamplePayloadFactory::ExamplePayloadFactory() {
}

ExamplePayloadFactory::~ExamplePayloadFactory() {
}

AbstractPayload* ExamplePayloadFactory::create(uint8_t* packetHeader) {
    ExamplePayload* payload = new ExamplePayload();
    return payload;
}

#endif