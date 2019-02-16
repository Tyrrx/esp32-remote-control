#ifndef REMOTECONTROL_PAYLOADFACTORYREGISTRY_H
#define REMOTECONTROL_PAYLOADFACTORYREGISTRY_H

//#include <AbstractPayload.h>
#include <AbstractPayloadFactory.h>

class PayloadFactoryRegistry {
   private:
    AbstractPayloadFactory *registry;

   public:
    PayloadFactoryRegistry(size_t factoryCount);
    ~PayloadFactoryRegistry();

    void registerFactory(AbstractPayloadFactory *abstractFactory);
};

PayloadFactoryRegistry::PayloadFactoryRegistry(size_t factoryCount) {
    this->registry = new AbstractPayloadFactory[factoryCount];
}

PayloadFactoryRegistry::~PayloadFactoryRegistry() {
}

void PayloadFactoryRegistry::registerFactory(AbstractPayloadFactory *abstractFactory) {
}

#endif