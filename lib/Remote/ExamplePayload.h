#ifndef REMOTECONTROL_EXAMPLEPAYLOAD_H
#define REMOTECONTROL_EXAMPLEPAYLOAD_H

#include <AbstractPayload.h>

#include "SSD1306.h"  // alias for `#include "SSD1306Wire.h"`
#define SDA 4
#define SCL 15
#define RST 16  //RST must be set by software
#define V2 1
#ifdef V2  //WIFI Kit series V1 not support Vext control
#define Vext 21
#endif

SSD1306 display(0x3c, SDA, SCL, RST);

class ExamplePayload : public AbstractPayload {
   private:
    const uint8_t headerSize = 0;
    uint8_t payloadSize = 0;
    const uint8_t type = PayloadType::EXAMPLE_PAYLOAD;

    String message;

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
};

ExamplePayload::ExamplePayload() {
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, LOW);  // OLED USE Vext as power supply, must turn ON Vext before OLED init
    //delay(50);
    display.init();
    display.flipScreenVertically();
    display.setContrast(255);
}

ExamplePayload::~ExamplePayload() {
}

bool ExamplePayload::execute(uint8_t* packetPayload) {
    this->message = String((char*)packetPayload);
    Serial.println(this->message);
    display.drawString(0, 0, this->message);
    display.display();
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
    this->payloadSize = string.length() + 1;
    this->message = string;
}

#endif