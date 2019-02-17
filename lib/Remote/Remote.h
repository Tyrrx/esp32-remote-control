//
// Created by David Retzlaff on 13.01.2019.
//

#ifndef REMOTECONTROL_REMOTE_H
#define REMOTECONTROL_REMOTE_H

#include <Arduino.h>
#include <ExamplePayloadFactory.h>
#include <LoRa.h>
#include <Packet.h>
#include <PacketBuilder.h>
#include <PayloadFactoryRegistry.h>
#include <PayloadType.h>
#include <esp_system.h>
#include <string.h>

//#include <Payload.h>

class Remote {
   public:
    Remote(uint8_t *key, uint32_t keySize);

    bool receive();

    bool send(AbstractPayload *abstractPayload);

   private:
    uint8_t syncWord;
    uint8_t txPower;
    bool isValid;

    PayloadFactoryRegistry *registry;
    PacketBuilder *packetBuilder;

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

Remote::Remote(uint8_t *key, uint32_t keySize) {
    this->syncWord = 0x8E;
    this->txPower = 10;
    this->isValid = this->setupLoRa();

    this->packetBuilder = new PacketBuilder(key, keySize);
    this->registry = new PayloadFactoryRegistry(1);
    this->registry->registerFactory(PayloadType::EXAMPLE_PAYLOAD, new ExamplePayloadFactory());
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
        Packet *packet = this->packetBuilder->create(packetSize);
        LoRa.readBytes(packet->getBuffer(), packet->getBufferSize());
        packet->setRssi(LoRa.packetRssi());
        if (this->packetBuilder->decode(packet)) {
            return this->registry->getFactory(packet->getPacketType())->create(packet->getHeader())->execute(packet->getPayload());
        }
    }
    return false;
}

bool Remote::send(AbstractPayload *abstractPayload) {
    Packet *packet = this->packetBuilder->encode(abstractPayload);
    LoRa.beginPacket();
    LoRa.write(packet->getBuffer(), packet->getBufferSize());
    LoRa.endPacket();
    return true;
}
/*
uint32_t Remote::send() {
    uint32_t time = millis();
    //uint8_t size = this->outputPacket->buildPacket();
    LoRa.beginPacket();
    //LoRa.write(this->outputPacket->getBuffer(), size);
    LoRa.endPacket();
    return millis() - time;
}
*/
/*
AuthenticatedCipher *Remote::getCipher() {
    this->cipher->clear();
    this->cipher->setKey(this->key, this->cipher->keySize());
    this->cipher->setIV(this->iv, this->cipher->ivSize());
    return this->cipher;
}
*/
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
