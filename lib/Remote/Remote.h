//
// Created by David Retzlaff on 13.01.2019.
//

#ifndef REMOTECONTROL_REMOTE_H
#define REMOTECONTROL_REMOTE_H

#include <Arduino.h>
#include <esp_system.h>

#include <ControlPayloadFactory.h>
#include <ExamplePayloadFactory.h>

#include <LoRa.h>
#include <PacketBuilder.h>
#include <PayloadFactoryRegistry.h>

#include <ControlService.h>
#include <FailsafeService.h>
#include <RemoteService.h>
#include <TelemetryService.h>

#include <Oled.h>

class Remote {
   public:
    Remote();
    ~Remote();
    // Receives data and executes it. Returns bool from AbstractPayload
    // execute().
    bool receive();
    void rec();

    // Sends a given specific Payload. Returns sent delay.
    uint32_t send(AbstractPayload* abstractPayload);

    PayloadFactoryRegistry* registry;
    PacketBuilder* packetBuilder;
    Oled* oled;
    LoRa* lora;

    FailsafeService* failsafeService;
    RemoteService* remoteService;

   private:
    // Key for encryption
    uint8_t keySize = 16;
    uint8_t key[16] = {0xfe, 0xaa, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c, 0x6d, 0xaa, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08};

    // Init status flag
    bool initStatus;
};

Remote::Remote() {
    this->oled = new Oled();
    this->lora = new LoRa();

    this->failsafeService = new FailsafeService();
    this->remoteService = new RemoteService();

    this->packetBuilder = new PacketBuilder();
    if (!this->packetBuilder->setCipherKey(this->key, this->keySize)) {
        this->initStatus = false;
    }
    this->registry = new PayloadFactoryRegistry(2);
    this->registry->registerFactory(PayloadType::EXAMPLE_PAYLOAD, new ExamplePayloadFactory(this->oled));
    this->registry->registerFactory(PayloadType::CONTROL_PAYLOAD, new ControlPayloadFactory(this->remoteService));
}

Remote::~Remote() {
    if (this->oled) {
        delete this->oled;
    }
    if (this->lora) {
        delete this->lora;
    }
    if (this->packetBuilder) {
        delete this->packetBuilder;
    }
    if (this->registry) {
        delete this->registry;
    }
}

bool Remote::receive() {
    uint16_t packetSize = this->lora->parsePacket();
    bool status = false;
    if (packetSize) {
        Packet* packet = this->packetBuilder->create(packetSize);
        this->lora->readBytes(packet->getBuffer(), packet->getBufferSize());
        packet->setRssi(this->lora->packetRssi());
        if (this->packetBuilder->decode(packet)) {
            AbstractPayload* payload = this->registry->getFactory(packet->getPacketType())->create(packet->getHeader());
            status = payload->execute(packet->getPayload());
            delete payload;
        }
        delete packet;
    }
    return status;
}

void Remote::rec() {}

uint32_t Remote::send(AbstractPayload* abstractPayload) {
    Packet* packet = this->packetBuilder->encode(abstractPayload);
    uint32_t sentDelay = this->lora->sendPacket(packet->getBuffer(), packet->getBufferSize());
    delete packet;
    return sentDelay;
}

#endif
