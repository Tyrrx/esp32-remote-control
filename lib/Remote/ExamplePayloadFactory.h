#ifndef REMOTECONTROL_EXAMPLEPAYLOADFACTORY_H
#define REMOTECONTROL_EXAMPLEPAYLOADFACTORY_H

#include <AbstractPayload.h>
#include <AbstractPayloadFactory.h>
#include <ExamplePayload.h>
#include <Oled.h>

class ExamplePayloadFactory : public AbstractPayloadFactory {
   private:
    Oled* oled;

   public:
    ExamplePayloadFactory(Oled* oled);
    ~ExamplePayloadFactory();

    // Creates a new ExamplePayload with given packetHeader.
    // Implements AbstractPayloadFactory create().
    AbstractPayload* create(uint8_t* packetHeader);
};

ExamplePayloadFactory::ExamplePayloadFactory(Oled* oled) { this->oled = oled; }

ExamplePayloadFactory::~ExamplePayloadFactory() {}

AbstractPayload* ExamplePayloadFactory::create(uint8_t* packetHeader) { return new ExamplePayload(this->oled); }

#endif