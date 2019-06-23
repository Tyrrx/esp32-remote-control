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
    PayloadFactoryRegistry();
    ~PayloadFactoryRegistry();

    // Registers a PayloadFactory with a type at the registry.
    void registerFactory(uint16_t type, AbstractPayloadFactory *abstractFactory);

    // Returns a specific PayloadFactory matching the type.
    AbstractPayloadFactory *getFactory(uint16_t type);

    void doubleFactoryArraySize();
};

PayloadFactoryRegistry::PayloadFactoryRegistry() {
    this->factoryCount = 8;
    this->abstractFactories = new AbstractPayloadFactory *[this->factoryCount];
}

PayloadFactoryRegistry::~PayloadFactoryRegistry() {
}

void PayloadFactoryRegistry::registerFactory(uint16_t type, AbstractPayloadFactory *abstractFactory) {
    if (type < this->factoryCount) {
        this->abstractFactories[type] = abstractFactory;
    } else() {
        this->doubleFactoryArraySize();
        this->registerFactory(type, abstractFactory);
    }
}

AbstractPayloadFactory *PayloadFactoryRegistry::getFactory(uint16_t type) {
    if (type < this->factoryCount) {
        return this->abstractFactories[type];
    }
    return NULL;
}

void PayloadFactoryRegistry::doubleFactoryArraySize() {
    AbstractPayloadFactory **newAbstractFactories = new AbstractPayloadFactory *[this->factoryCount*2];
    memcpy(newAbstractFactories,this->abstractFactories, this->factoryCount * sizeof(AbstractPayloadFactory));
    this->abstractFactories = newAbstractFactories;
    this->this->factoryCount = this->factoryCount*2;
}

#endif