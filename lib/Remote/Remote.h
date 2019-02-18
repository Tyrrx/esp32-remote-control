//
// Created by David Retzlaff on 13.01.2019.
//

#ifndef REMOTECONTROL_REMOTE_H
#define REMOTECONTROL_REMOTE_H

#include <Arduino.h>
#include <LoRa.h>
#include <esp_system.h>

#include <PacketBuilder.h>
#include <PayloadFactoryRegistry.h>

class Remote {
   public:
    Remote(uint8_t *key, uint32_t keySize);

    bool receive();

    bool send(AbstractPayload *abstractPayload);

    PayloadFactoryRegistry *registry;
    PacketBuilder *packetBuilder;

   private:
    // General settings of WIFI LoRa 32
    const uint8_t syncWord = 0x8E;
    const bool paboost = true;
    const double band = 868E6;
    const double bandwidth = 250E3;
    const uint8_t spreadingFactor = 7;

    // Pin definetion of WIFI LoRa 32
    const uint8_t sck = 5;    // GPIO5  -- SX1278's SCK
    const uint8_t miso = 19;  // GPIO19 -- SX1278's MISO
    const uint8_t mosi = 27;  // GPIO27 -- SX1278's MOSI
    const uint8_t ss = 18;    // GPIO18 -- SX1278's CS
    const uint8_t rst = 14;   // GPIO14 -- SX1278's RESET
    const uint8_t di0 = 26;   // GPIO26 -- SX1278's IRQ(Interrupt Request)

    uint8_t txPower;

    bool valid;

    bool setupLoRa();
};

Remote::Remote(uint8_t *key, uint32_t keySize) {
    this->txPower = 10;
    this->valid = this->setupLoRa();

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
    bool status = false;
    if (packetSize) {
        Packet *packet = this->packetBuilder->create(packetSize);
        //SmartPtr<Packet> packet(this->packetBuilder->create(packetSize));
        LoRa.readBytes(packet->getBuffer(), packet->getBufferSize());
        packet->setRssi(LoRa.packetRssi());
        if (this->packetBuilder->decode(packet)) {
            AbstractPayload *payload = this->registry->getFactory(packet->getPacketType())->create(packet->getHeader());
            status = payload->execute(packet->getPayload());
            delete payload;
        }
        delete packet;
    }
    return status;
}

bool Remote::send(AbstractPayload *abstractPayload) {
    uint32_t time = millis();
    Packet *packet = this->packetBuilder->encode(abstractPayload);
    LoRa.beginPacket();
    LoRa.write(packet->getBuffer(), packet->getBufferSize());
    LoRa.endPacket();
    time = millis() - time;
    delete packet;
    return true;
}

bool Remote::setupLoRa() {
    SPI.begin(this->sck, this->miso, this->mosi, this->ss);
    LoRa.setPins(this->ss, this->rst, this->di0);
    LoRa.setPins(this->ss, this->rst, this->di0);
    if (!LoRa.begin(this->band, this->paboost)) {
        return false;
    }
    LoRa.setTxPower(this->txPower, 2);
    LoRa.setSyncWord(this->syncWord);
    LoRa.setSignalBandwidth(this->bandwidth);
    LoRa.setSpreadingFactor(this->spreadingFactor);
    return true;
}

#endif
