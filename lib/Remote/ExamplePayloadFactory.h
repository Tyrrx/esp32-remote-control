#ifndef REMOTECONTROL_EXAMPLEPAYLOADFACTORY_H
#define REMOTECONTROL_EXAMPLEPAYLOADFACTORY_H

#include <AbstractPayload.h>
#include <AbstractPayloadFactory.h>
#include <ExamplePayload.h>
#include <U8g2lib.h>

class ExamplePayloadFactory : public AbstractPayloadFactory {
   private:
    U8G2* oled;

   public:
    ExamplePayloadFactory(U8G2* oled);
    ~ExamplePayloadFactory();

    // Creates a new ExamplePayload with given packetHeader.
    // Implements AbstractPayloadFactory create().
    AbstractPayload* create(uint8_t* packetHeader);
};

ExamplePayloadFactory::ExamplePayloadFactory(U8G2* oled) {
    this->oled = oled;
}

ExamplePayloadFactory::~ExamplePayloadFactory() {
}

AbstractPayload* ExamplePayloadFactory::create(uint8_t* packetHeader) {
    ExamplePayload* payload = new ExamplePayload();
    payload->setOled(this->oled);
    return payload;
}

#endif