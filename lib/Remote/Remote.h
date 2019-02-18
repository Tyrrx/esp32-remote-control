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

#include <U8g2lib.h>
#include <Wire.h>

class Remote {
   public:
    Remote();

    // Receives data and executes it. Returns bool from AbstractPayload execute().
    bool receive();

    //
    void receiveCallback(int packetSize);

    // Sends a given specific Payload. Returns sent delay.
    uint32_t send(AbstractPayload *abstractPayload);

    // Returns true if remote setup was successful.
    bool initSuccsess();

    PayloadFactoryRegistry *registry;
    PacketBuilder *packetBuilder;
    U8G2 *oled;

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

    // Pin definetion of OLED
    const uint8_t sda = 4;
    const uint8_t scl = 15;
    const uint8_t rstOled = 16;

    // Key for encryption
    uint8_t keySize = 16;
    uint8_t key[16] = {0xfe, 0xaa, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c,
                       0x6d, 0xaa, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08};

    // Transmission power
    uint8_t txPower;

    // Init status flag
    bool initStatus;

    // Sets up LoRa library and hardware.
    bool setupLoRa();
};

Remote::Remote() {
    this->oled = new U8G2_SSD1306_128X64_NONAME_F_SW_I2C(U8G2_R0, this->scl, this->sda, this->rstOled);
    this->oled->begin();
    this->oled->setFont(u8g2_font_5x8_mf);

    this->txPower = 10;
    this->initStatus = this->setupLoRa();

    this->packetBuilder = new PacketBuilder();
    if (!this->packetBuilder->setCipherKey(this->key, this->keySize)) {
        this->initStatus = false;
    }
    this->registry = new PayloadFactoryRegistry(1);
    this->registry->registerFactory(PayloadType::EXAMPLE_PAYLOAD, new ExamplePayloadFactory(this->oled));
}

bool Remote::receive() {
    uint16_t packetSize = LoRa.parsePacket();
    bool status = false;
    if (packetSize) {
        Packet *packet = this->packetBuilder->create(packetSize);
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

uint32_t Remote::send(AbstractPayload *abstractPayload) {
    uint32_t time = millis();
    Packet *packet = this->packetBuilder->encode(abstractPayload);
    LoRa.beginPacket();
    LoRa.write(packet->getBuffer(), packet->getBufferSize());
    LoRa.endPacket();
    delete packet;
    return millis() - time;
}

bool Remote::initSuccsess() {
    return this->initStatus;
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
