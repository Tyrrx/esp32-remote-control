#ifndef REMOTECONTROL_PACKETBUILDER_H
#define REMOTECONTROL_PACKETBUILDER_H

#include <AbstractPayload.h>
#include <Packet.h>

#include <AES.h>
#include <Crypto.h>
#include <CryptoLW.h>
#include <GCM.h>
#include <pgmspace.h>

class PacketBuilder {
   private:
    uint8_t* iv;
    uint8_t* key;

    uint32_t ivSize;
    uint32_t keySize;

    AuthenticatedCipher* cipher;

    void parsePacketBuffer(Packet* packet);
    void buildPacketBuffer(Packet* packet);
    void parsePacketHeader(Packet* packet);
    void buildPacketHeader(Packet* packet);
    bool encryptPacket(Packet* packet);
    bool decryptPacket(Packet* packet);
    void prepareCipher();

   public:
    PacketBuilder(uint8_t* key, uint32_t keySize);
    ~PacketBuilder();

    Packet* create(uint8_t bufferSize);

    Packet* create();

    Packet* encode(AbstractPayload* abstractPayload);

    bool decode(Packet* packet);
};

PacketBuilder::PacketBuilder(uint8_t* key, uint32_t keySize) {
    this->cipher = new GCM<AES128>;
    if (this->cipher->keySize() == keySize) {
        this->key = new uint8_t[this->cipher->keySize()];
        this->iv = new uint8_t[this->cipher->ivSize()];
        memcpy(this->key, key, keySize);
        //create iv
        this->keySize = keySize;
        this->ivSize = this->cipher->ivSize();
    }
}

PacketBuilder::~PacketBuilder() {
    if (this->iv) {
        delete this->iv;
    }
    if (this->key) {
        delete this->key;
    }
    if (this->cipher) {
        delete this->cipher;
    }
}

Packet* PacketBuilder::create(uint8_t bufferSize) {
    Packet* packet = new Packet();
    packet->createBuffer(bufferSize);
    return packet;
}

Packet* PacketBuilder::create() {
    return new Packet();
}

Packet* PacketBuilder::encode(AbstractPayload* abstractPayload) {
    Packet* packet = this->create();
    abstractPayload->build(packet);
    this->buildPacketHeader(packet);
    this->encryptPacket(packet);
    this->buildPacketBuffer(packet);
    return packet;
}

bool PacketBuilder::decode(Packet* packet) {
    this->parsePacketHeader(packet);
    this->parsePacketBuffer(packet);
    return this->encryptPacket(packet);
}

void PacketBuilder::parsePacketBuffer(Packet* packet) {
    packet->createHashBuffer(this->cipher->tagSize());
    uint8_t startIndex = 0;
    memcpy(packet->getPacketHeader(), &packet->getBuffer()[startIndex], packet->getPacketHeaderSize() * sizeof(uint8_t));
    startIndex += packet->getPacketHeaderSize();
    memcpy(packet->getHeader(), &packet->getBuffer()[startIndex], packet->getHeaderSize() * sizeof(uint8_t));
    startIndex += packet->getHeaderSize();
    memcpy(packet->getHash(), &packet->getBuffer()[startIndex], packet->getHashSize() * sizeof(uint8_t));
    startIndex += packet->getHashSize();
    memcpy(packet->getPayload(), &packet->getBuffer()[startIndex], packet->getPayloadSize() * sizeof(uint8_t));
}

void PacketBuilder::buildPacketBuffer(Packet* packet) {
    packet->createBuffer(packet->getPacketHeaderSize() + packet->getHeaderSize() + packet->getHashSize() + packet->getPayloadSize());
    uint8_t startIndex = 0;
    memcpy(&packet->getBuffer()[startIndex], packet->getPacketHeader(), packet->getPacketHeaderSize() * sizeof(uint8_t));
    startIndex += packet->getPacketHeaderSize();
    memcpy(&packet->getBuffer()[startIndex], packet->getHeader(), packet->getHeaderSize() * sizeof(uint8_t));
    startIndex += packet->getHeaderSize();
    memcpy(&packet->getBuffer()[startIndex], packet->getHash(), packet->getHashSize() * sizeof(uint8_t));
    startIndex += packet->getHashSize();
    memcpy(&packet->getBuffer()[startIndex], packet->getPayload(), packet->getPayloadSize() * sizeof(uint8_t));
}

void PacketBuilder::parsePacketHeader(Packet* packet) {
    packet->createHeaderBuffer(packet->getBuffer()[0]);
    packet->createPayloadBuffer(packet->getBuffer()[1]);
    packet->setPacketType(packet->getBuffer()[2]);
}

void PacketBuilder::buildPacketHeader(Packet* packet) {
    packet->getHeader()[0] = packet->getHeaderSize();
    packet->getHeader()[1] = packet->getPayloadSize();
    packet->getHeader()[2] = packet->getPacketType();
}

bool PacketBuilder::encryptPacket(Packet* packet) {
    this->prepareCipher();
    this->cipher->addAuthData(packet->getPacketHeader(), packet->getPacketHeaderSize());
    this->cipher->addAuthData(packet->getHeader(), packet->getHeaderSize());
    this->cipher->encrypt(packet->getPayload(), packet->getPayload(), packet->getPayloadSize());
    this->cipher->computeTag(packet->getHash(), packet->getHashSize());
    return true;
}

bool PacketBuilder::decryptPacket(Packet* packet) {
    this->prepareCipher();
    this->cipher->addAuthData(packet->getPacketHeader(), packet->getPacketHeaderSize());
    this->cipher->addAuthData(packet->getHeader(), packet->getHeaderSize());
    this->cipher->decrypt(packet->getPayload(), packet->getPayload(), packet->getPayloadSize());
    if (this->cipher->checkTag(packet->getHash(), packet->getHashSize())) {
        return true;
    }
    return false;
}

void PacketBuilder::prepareCipher() {
    this->cipher->clear();
    this->cipher->setKey(this->key, this->cipher->keySize());
    this->cipher->setIV(this->iv, this->cipher->ivSize());
}

#endif