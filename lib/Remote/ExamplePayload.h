#ifndef REMOTECONTROL_EXAMPLEPAYLOAD_H
#define REMOTECONTROL_EXAMPLEPAYLOAD_H

#include <AbstractPayload.h>
#include <U8g2lib.h>

class ExamplePayload : public AbstractPayload {
   private:
    const uint8_t headerSize = 0;
    uint8_t payloadSize = 0;
    const uint8_t type = PayloadType::EXAMPLE_PAYLOAD;

    String message;
    U8G2* oled;

   public:
    ExamplePayload();
    ~ExamplePayload();

    // Executes ExamplePayload with given data.
    // Implements AbstractPayload execute().
    bool execute(uint8_t* packetPayload);

    // Builds a given Packets data.
    // Implements AbstractPayload build().
    bool build(Packet* packet);

    // Sets a value.
    void setString(String string);

    // Inject oled;
    void setOled(U8G2* oled);
};

ExamplePayload::ExamplePayload() {
}

ExamplePayload::~ExamplePayload() {
}

bool ExamplePayload::execute(uint8_t* packetPayload) {
    this->message = String((char*)packetPayload);
    Serial.println(this->message);
    //experimental function body
    uint8_t row = this->oled->getMaxCharHeight();
    uint16_t pixelDrawn = 0;
    this->oled->setCursor(0, row);
    for (size_t i = 0; i < this->message.length() + 1; i++) {
        if (pixelDrawn + this->oled->getMaxCharWidth() >= this->oled->getDisplayWidth()) {
            row += this->oled->getMaxCharHeight() + 2;
            pixelDrawn = 0;
            this->oled->setCursor(0, row);
        }
        pixelDrawn += this->oled->getMaxCharWidth();
        this->oled->print(this->message.c_str()[i]);
    }
    this->oled->sendBuffer();
    return true;
}

bool ExamplePayload::build(Packet* packet) {
    this->payloadSize = this->message.length() + 1;

    packet->setPacketType(this->type);
    packet->createHeaderBuffer(this->headerSize);
    packet->createPayloadBuffer(this->payloadSize);

    this->message.getBytes(packet->getPayload(), packet->getPayloadSize());
    return true;
}

void ExamplePayload::setString(String string) {
    this->message = string;
}

void ExamplePayload::setOled(U8G2* oled) {
    this->oled = oled;
    this->oled->clearBuffer();
}

#endif