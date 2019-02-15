//
// Created by David Retzlaff on 13.01.2019.
//

#ifndef REMOTECONTROL_REMOTE_H
#define REMOTECONTROL_REMOTE_H

#include <Arduino.h>
#include <ExamplePacket.h>
#include <LoRa.h>
#include <Packet.h>
#include <esp_system.h>
#include <string.h>

//#include <Payload.h>

class Remote {
   public:
    Remote(AuthenticatedCipher *cipher, uint8_t *key, uint32_t keySize);

    void prepareCipher();

    bool receive();

    Packet *getInputPacket();

    Packet *createOutputPacket();

    uint32_t send();

    void createIv();

    AuthenticatedCipher *getCipher();

    void handlePacket(Packet packet);
    void registerPacket();

   private:
    uint8_t syncWord;
    uint8_t txPower;

    Packet *inputPacket;
    Packet *outputPacket;

    AuthenticatedCipher *cipher;
    //    Random *random;
    uint8_t *key;
    uint8_t *iv;
    bool isValid;

   private:
    bool setupLoRa();
};

// Pin definetion of WIFI LoRa 32
#define SCK 5    // GPIO5  -- SX1278's SCK
#define MISO 19  // GPIO19 -- SX1278's MISO
#define MOSI 27  // GPIO27 -- SX1278's MOSI
#define SS 18    // GPIO18 -- SX1278's CS
#define RST 14   // GPIO14 -- SX1278's RESET
#define DI0 26   // GPIO26 -- SX1278's IRQ(Interrupt Request)

#define BAND 868E6  //you can set band here directly,e.g. 868E6,915E6
#define PABOOST true

Remote::Remote(AuthenticatedCipher *cipher, uint8_t *key, uint32_t keySize) {
    this->syncWord = 0x8E;
    this->txPower = 10;

    this->isValid = this->setupLoRa();

    //this->random = new Random();
    this->cipher = cipher;

    if (this->cipher->keySize() == keySize) {
        this->key = new uint8_t[this->cipher->keySize()];
        this->iv = new uint8_t[this->cipher->ivSize()];
        memcpy(this->key, key, keySize);
        this->createIv();
        this->isValid = true;
    }
}

void Remote::prepareCipher() {
    this->cipher->clear();
    this->cipher->setKey(this->key, this->cipher->keySize());
    this->cipher->setIV(this->iv, this->cipher->ivSize());
}
/*
bool Remote::updateControl(int *channels, int size) {
    if (size <= channelCount) {
        for (int i = 0; i < size; ++i) {
            int val = channels[i];
            controlBytes[2 * i] = (byte)(val & 0xff);
            controlBytes[2 * i + 1] = (byte)(val >> 8);
        }
        return true;
    }
    return false;
}
*/

bool Remote::receive() {
    uint16_t packetSize = LoRa.parsePacket();
    if (packetSize) {
        this->prepareCipher();
        this->inputPacket = new Packet(this->cipher, packetSize);
        LoRa.readBytes(inputPacket->getBuffer(), packetSize);
        this->inputPacket->setRssi(LoRa.packetRssi());
        return this->inputPacket->validatePacket();
    }
    return false;
}

Packet *Remote::getInputPacket() {
    return this->inputPacket;
}
/*
Packet *Remote::createOutputPacket() {
    this->outputPacket = new Packet(this->cipher);
    return this->outputPacket;
}
*/

uint32_t Remote::send() {
    uint32_t time = millis();
    this->prepareCipher();
    uint8_t size = this->outputPacket->buildPacket();
    LoRa.beginPacket();
    LoRa.write(this->outputPacket->getBuffer(), size);
    LoRa.endPacket();
    return millis() - time;
}
void Remote::createIv() {
    uint32_t size = this->cipher->ivSize();
    for (uint8_t i = 0; i < size; i++) {
        this->iv[i] = 255;
    }

    //this->random->randomBytes(this->iv, size);
}

AuthenticatedCipher *Remote::getCipher() {
    return this->cipher;
}

bool Remote::setupLoRa() {
    SPI.begin(SCK, MISO, MOSI, SS);
    LoRa.setPins(SS, RST, DI0);
    LoRa.setPins(SS, RST, DI0);
    if (!LoRa.begin(BAND, PABOOST)) {
        return false;
    }
    LoRa.setTxPower(this->txPower, 2);
    LoRa.setSyncWord(this->syncWord);
    LoRa.setSignalBandwidth(250E3);
    LoRa.setSpreadingFactor(7);
    return true;
}

#endif
