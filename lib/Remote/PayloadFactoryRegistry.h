#ifndef REMOTECONTROL_PAYLOADFACTORYREGISTRY_H
#define REMOTECONTROL_PAYLOADFACTORYREGISTRY_H

#include <AbstractPayloadFactory.h>
#include <ExamplePayloadFactory.h>
#include <PayloadType.h>

class PayloadFactoryRegistry {
   private:
    AbstractPayloadFactory **abstractFactories;
    uint16_t factoryCount;

   public:
    PayloadFactoryRegistry(uint16_t count);
    ~PayloadFactoryRegistry();

    void registerFactory(uint16_t type, AbstractPayloadFactory *abstractFactory);

    AbstractPayloadFactory *getFactory(uint16_t type);
};

PayloadFactoryRegistry::PayloadFactoryRegistry(uint16_t count) {
    this->abstractFactories = new AbstractPayloadFactory *[count];
    this->factoryCount = count;
}

PayloadFactoryRegistry::~PayloadFactoryRegistry() {
}

void PayloadFactoryRegistry::registerFactory(uint16_t type, AbstractPayloadFactory *abstractFactory) {
    if (type < this->factoryCount) {
        this->abstractFactories[type] = abstractFactory;
    }
}

AbstractPayloadFactory *PayloadFactoryRegistry::getFactory(uint16_t type) {
    if (type < this->factoryCount) {
        return this->abstractFactories[type];
    }
    return NULL;
}

#endif