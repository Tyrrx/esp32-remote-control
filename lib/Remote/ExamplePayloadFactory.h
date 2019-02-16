#ifndef REMOTECONTROL_EXAMPLEPAYLOADFACTORY_H
#define REMOTECONTROL_EXAMPLEPAYLOADFACTORY_H

#include <AbstractPayload.h>
#include <AbstractPayloadFactory.h>

class ExamplePayloadFactory : AbstractPayloadFactory {
   private:
   public:
    ExamplePayloadFactory();
    ~ExamplePayloadFactory();
    AbstractPayload* create();
};

ExamplePayloadFactory::ExamplePayloadFactory() {
}

ExamplePayloadFactory::~ExamplePayloadFactory() {
}

AbstractPayload* ExamplePayloadFactory::create() {
}

#endif