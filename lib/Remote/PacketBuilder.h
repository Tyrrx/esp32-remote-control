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
    PacketBuilder();
    ~PacketBuilder();

    // Creates a new Packet with specific buffer size.
    Packet* create(uint8_t bufferSize);

    // Creates a new Packet without specific buffer size.
    Packet* create();

    // Encodes a Payload. Returns a new Packet.
    Packet* encode(AbstractPayload* abstractPayload);

    // Decodes a given Packet.
    bool decode(Packet* packet);

    // Setup a cipher key. returns true if keysize is valid.
    bool setCipherKey(uint8_t* key, uint32_t keySize);
};

PacketBuilder::PacketBuilder() {
    this->cipher = new GCM<AES128>;
    this->keySize = this->cipher->keySize();
    this->ivSize = this->cipher->ivSize();
    this->key = new uint8_t[this->keySize];
    this->iv = new uint8_t[this->ivSize];
    //replace with random iv
    for (size_t i = 0; i < this->ivSize; i++) {
        this->iv[i] = 255;
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
    return this->decryptPacket(packet);
}

void PacketBuilder::parsePacketBuffer(Packet* packet) {
    packet->createHashBuffer(this->cipher->tagSize());
    uint8_t startIndex = 0;
    memcpy(packet->getPacketHeader(), &packet->getBuffer()[startIndex], packet->getPacketHeaderSize() * sizeof(uint8_t));
    startIndex += packet->getPacketHeaderSize() * sizeof(uint8_t);
    memcpy(packet->getHeader(), &packet->getBuffer()[startIndex], packet->getHeaderSize() * sizeof(uint8_t));
    startIndex += packet->getHeaderSize() * sizeof(uint8_t);
    memcpy(packet->getHash(), &packet->getBuffer()[startIndex], packet->getHashSize() * sizeof(uint8_t));
    startIndex += packet->getHashSize() * sizeof(uint8_t);
    memcpy(packet->getPayload(), &packet->getBuffer()[startIndex], packet->getPayloadSize() * sizeof(uint8_t));
}

void PacketBuilder::buildPacketBuffer(Packet* packet) {
    packet->createBuffer(packet->getPacketHeaderSize() + packet->getHeaderSize() + packet->getHashSize() + packet->getPayloadSize());
    uint8_t startIndex = 0;
    memcpy(&packet->getBuffer()[startIndex], packet->getPacketHeader(), packet->getPacketHeaderSize() * sizeof(uint8_t));
    startIndex += packet->getPacketHeaderSize() * sizeof(uint8_t);
    memcpy(&packet->getBuffer()[startIndex], packet->getHeader(), packet->getHeaderSize() * sizeof(uint8_t));
    startIndex += packet->getHeaderSize() * sizeof(uint8_t);
    memcpy(&packet->getBuffer()[startIndex], packet->getHash(), packet->getHashSize() * sizeof(uint8_t));
    startIndex += packet->getHashSize() * sizeof(uint8_t);
    memcpy(&packet->getBuffer()[startIndex], packet->getPayload(), packet->getPayloadSize() * sizeof(uint8_t));
}

void PacketBuilder::parsePacketHeader(Packet* packet) {
    packet->createHeaderBuffer(packet->getBuffer()[Packet::HEADER_SIZE_ADDRESS]);
    packet->createPayloadBuffer(packet->getBuffer()[Packet::PAYLOAD_SIZE_ADDRESS]);
    packet->setPacketType(packet->getBuffer()[Packet::TYPE_ADDRESS]);
}

void PacketBuilder::buildPacketHeader(Packet* packet) {
    packet->getPacketHeader()[Packet::HEADER_SIZE_ADDRESS] = packet->getHeaderSize();
    packet->getPacketHeader()[Packet::PAYLOAD_SIZE_ADDRESS] = packet->getPayloadSize();
    packet->getPacketHeader()[Packet::TYPE_ADDRESS] = packet->getPacketType();
}

bool PacketBuilder::encryptPacket(Packet* packet) {
    this->prepareCipher();
    this->cipher->addAuthData(packet->getPacketHeader(), packet->getPacketHeaderSize());
    this->cipher->addAuthData(packet->getHeader(), packet->getHeaderSize());
    this->cipher->encrypt(packet->getPayload(), packet->getPayload(), packet->getPayloadSize());
    packet->createHashBuffer(this->cipher->tagSize());
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

bool PacketBuilder::setCipherKey(uint8_t* key, uint32_t keySize) {
    if (this->cipher->keySize() == keySize) {
        memcpy(this->key, key, keySize);
        return true;
    }
    return false;
}
#endif