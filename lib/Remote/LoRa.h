#ifndef REMOTECONTROL_LORA_H
#define REMOTECONTROL_LORA_H

#include <Arduino.h>
#include <LoRaDriver.h>

class LoRa : public LoRaDriver {
   private:
    // General settings of WIFI LoRa 32
    const uint8_t syncWord = 0x8E;
    const bool paboost = true;
    const double band = 868E6;
    const double bandwidth = 250E3;
    const uint8_t spreadingFactor = 7;
    uint8_t txPower = 10;

    // Pin definetion of WIFI LoRa 32
    const uint8_t sck = 5;    // GPIO5  -- SX1278's SCK
    const uint8_t miso = 19;  // GPIO19 -- SX1278's MISO
    const uint8_t mosi = 27;  // GPIO27 -- SX1278's MOSI
    const uint8_t ss = 18;    // GPIO18 -- SX1278's CS
    const uint8_t rst = 14;   // GPIO14 -- SX1278's RESET
    const uint8_t di0 = 26;   // GPIO26 -- SX1278's IRQ(Interrupt Request)

    bool initStatus;

   public:
    LoRa();
    ~LoRa();

    bool initSucceed();

    uint8_t sendPacket(uint8_t* buf, uint8_t size);
};

LoRa::LoRa() {
    SPI.begin(this->sck, this->miso, this->mosi, this->ss);
    this->setPins(this->ss, this->rst, this->di0);
    this->setPins(this->ss, this->rst, this->di0);
    if (!this->begin(this->band, this->paboost)) {
        this->initStatus = false;
    }
    this->setTxPower(this->txPower, 2);
    this->setSyncWord(this->syncWord);
    this->setSignalBandwidth(this->bandwidth);
    this->setSpreadingFactor(this->spreadingFactor);
}

LoRa::~LoRa() { this->end(); }

bool LoRa::initSucceed() { return this->initStatus; }

uint8_t LoRa::sendPacket(uint8_t* buf, uint8_t size) {
    uint32_t delta = millis();
    this->beginPacket();
    this->write(buf, size);
    this->endPacket();
    return delta - millis();
}

#endif