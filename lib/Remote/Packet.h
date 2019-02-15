#ifndef REMOTECONTROL_PACKET_H
#define REMOTECONTROL_PACKET_H

#include <AES.h>
#include <Arduino.h>
#include <Crypto.h>
#include <CryptoLW.h>
#include <GCM.h>
#include <pgmspace.h>
#include <string.h>

class Packet {
   protected:
    bool valid;

    bool hasBuffer;
    bool hasPayloadBuffer;
    bool hasHashBuffer;
    bool hasHeaderBuffer;
    bool hasPacketHeaderBuffer;

    bool hasBufferData;
    bool hasPayload;
    bool hasHash;
    bool hasHeader;

    uint8_t packetType;

    uint8_t *buffer;
    uint8_t *payload;
    uint8_t *hash;
    uint8_t *header;
    uint8_t *packetHeader;

    uint8_t bufferSize;
    uint8_t payloadSize;
    uint8_t hashSize;
    uint8_t headerSize;
    uint8_t packetHeaderSize;

    uint8_t maxBufferSize;
    uint8_t maxPayloadSize;

    int16_t rssi;
    uint32_t timestamp;
    AuthenticatedCipher *cipher;

    bool parseBuffer();

   public:
    Packet(AuthenticatedCipher *cipher, uint8_t bufferSize);
    ~Packet();

    void setPacketType(uint8_t type);
    uint8_t getPacketType();

    bool prepareBuffer(uint8_t size);
    bool preparePacketHeaderBuffer();
    bool prepareHeaderBuffer();
    bool prepareHashBuffer();
    bool preparePayloadBuffer();

    bool validatePacket();
    uint8_t buildPacket();

    virtual bool build();
    virtual bool parse();

    bool isEmpty();
    bool isValid();

    uint8_t *getBuffer();
    uint8_t *getPayload();
    uint8_t *getHash();
    uint8_t *getHeader();
    uint8_t *getPacketHeader();

    void setBufferSize(uint8_t size);
    void setPayloadSize(uint8_t size);
    void setHashSize(uint8_t size);
    void setHeaderSize(uint8_t size);
    void setPacketHeaderSize(uint8_t size);

    uint8_t getBufferSize();
    uint8_t getPayloadSize();
    uint8_t getHashSize();
    uint8_t getHeaderSize();
    uint8_t getPacketHeaderSize();

    uint8_t getMaxBufferSize();
    uint8_t getMaxPayloadSize();

    void setRssi(int16_t rssi);
    int16_t getRssi();

    uint32_t getTimestamp();
    void setTimestamp();
};

Packet::Packet(AuthenticatedCipher *cipher, uint8_t bufferSize) {
    this->setTimestamp();
    this->cipher = cipher;
    this->setHashSize(this->cipher->tagSize());

    this->maxBufferSize = 255;
    this->maxPayloadSize = 64;

    this->valid = false;

    this->hasBuffer = false;
    this->hasPacketHeaderBuffer = false;
    this->hasHeaderBuffer = false;
    this->hasHashBuffer = false;
    this->hasPayloadBuffer = false;

    this->hasBufferData = false;
    this->hasPayload = false;
    this->hasHash = false;
    this->hasHeader = false;

    this->setPacketHeaderSize(3);
    this->prepareBuffer(bufferSize);
}

Packet::~Packet() {
    if (this->hasBuffer) {
        delete this->buffer;
    }
    if (this->hasPacketHeaderBuffer) {
        delete this->packetHeader;
    }
    if (this->hasHeaderBuffer) {
        delete this->header;
    }
    if (this->hasHashBuffer) {
        delete this->hash;
    }
    if (this->hasPayloadBuffer) {
        delete this->payload;
    }
}
bool Packet::parseBuffer() {
    if (this->hasBuffer && this->preparePacketHeaderBuffer() && this->prepareHeaderBuffer() && this->prepareHashBuffer() && this->preparePayloadBuffer()) {
        this->setHeaderSize(this->buffer[0]);
        this->setPayloadSize(this->buffer[1]);
        this->setPacketType(this->buffer[2]);
        uint8_t startIndex = 0;
        for (uint8_t i = 0; i < this->getPacketHeaderSize(); i++) {
            this->packetHeader[i] = this->buffer[i + startIndex];
        }
        startIndex += this->getPacketHeaderSize();
        for (uint8_t i = 0; i < this->getHeaderSize(); i++) {
            this->header[i] = this->buffer[i + startIndex];
        }
        startIndex += this->getHeaderSize();
        for (uint8_t i = 0; i < this->getHashSize(); i++) {
            this->hash[i] = this->buffer[i + startIndex];
        }
        startIndex += this->getHashSize();
        for (uint8_t i = 0; i < this->getPayloadSize(); i++) {
            this->payload[i] = this->buffer[i + startIndex];
        }
        return true;
    }
    return false;
}

bool Packet::preparePacketHeaderBuffer() {
    if (!this->hasPacketHeaderBuffer && this->getPacketHeaderSize() > 0) {
        this->packetHeader = new uint8_t[this->getPacketHeaderSize()];
        return this->hasPacketHeaderBuffer = true;
    }
    return false;
}

bool Packet::prepareHeaderBuffer() {
    if (!this->hasHeaderBuffer && this->getHeaderSize() > 0) {
        this->header = new uint8_t[this->getHeaderSize()];
        return this->hasHeaderBuffer = true;
    }
    return false;
}

bool Packet::prepareHashBuffer() {
    if (!this->hasHashBuffer && this->getHashSize() > 0) {
        this->hash = new uint8_t[this->getHashSize()];
        return this->hasHashBuffer = true;
    }
    return false;
}

bool Packet::preparePayloadBuffer() {
    if (!this->hasPayloadBuffer && this->getPayloadSize() > 0) {
        this->payload = new uint8_t[this->getPayloadSize()];
        return this->hasPayloadBuffer = true;
    }
    return false;
}

bool Packet::prepareBuffer(uint8_t size) {
    if (!this->hasBuffer && size > 0 && size <= this->getMaxBufferSize()) {
        this->setBufferSize(size);
        this->buffer = new uint8_t[this->getBufferSize()];
        return this->hasBuffer = true;
    }
    return false;
}

bool Packet::validatePacket() {
    if (this->parseBuffer()) {
        this->cipher->addAuthData(this->packetHeader, this->getPacketHeaderSize());
        this->cipher->addAuthData(this->header, this->getHeaderSize());
        this->cipher->decrypt(this->payload, this->payload, this->getPayloadSize());
        if (this->cipher->checkTag(this->hash, this->getHashSize())) {
            this->valid = true;
        }
        return this->isValid();
    }
    return false;
}
uint8_t Packet::buildPacket() {
    if (this->hasHeaderBuffer &&
        this->hasPayloadBuffer &&
        //  this->hasHeader &&
        // this->hasPayload &&
        this->prepareBuffer(this->getPacketHeaderSize() + this->getHeaderSize() + this->getHashSize() + this->getPayloadSize()) &&
        this->prepareHashBuffer() &&
        this->preparePacketHeaderBuffer()) {
        //build the packet

        this->packetHeader[0] = this->getHeaderSize();
        this->packetHeader[1] = this->getPayloadSize();
        this->packetHeader[2] = this->getPacketType();

        this->cipher->addAuthData(this->packetHeader, this->getPacketHeaderSize());
        this->cipher->addAuthData(this->header, this->getHeaderSize());
        this->cipher->encrypt(this->payload, this->payload, this->getPayloadSize());
        this->cipher->computeTag(this->hash, this->getHashSize());

        /*
        memcpy(this->buffer, this->packetHeader, this->getPacketHeaderSize() * sizeof(uint8_t));
        memcpy(this->buffer + this->getPacketHeaderSize(), this->header, this->getHeaderSize() * sizeof(uint8_t));
        memcpy(this->buffer + this->getPacketHeaderSize() + this->getHeaderSize(), this->hash, this->getHashSize() * sizeof(uint8_t));
        memcpy(this->buffer + this->getPacketHeaderSize() + this->getHeaderSize() + this->getHashSize(), this->payload, this->getPayloadSize() * sizeof(uint8_t));
        */

        uint8_t startIndex = 0;
        for (uint8_t i = 0; i < this->getPacketHeaderSize(); i++) {
            this->buffer[i + startIndex] = this->packetHeader[i];
        }
        startIndex += this->getPacketHeaderSize();
        for (uint8_t i = 0; i < this->getHeaderSize(); i++) {
            this->buffer[i + startIndex] = this->header[i];
        }
        startIndex += this->getHeaderSize();
        for (uint8_t i = 0; i < this->getHashSize(); i++) {
            this->buffer[i + startIndex] = this->hash[i];
        }
        startIndex += this->getHashSize();
        for (uint8_t i = 0; i < this->getPayloadSize(); i++) {
            this->buffer[i + startIndex] = this->payload[i];
        }
        return this->getBufferSize();
    }
    return 0;
}

bool Packet::isEmpty() { return !this->hasBuffer && !this->hasHashBuffer && !this->hasPacketHeaderBuffer && !this->hasHeaderBuffer && !this->hasPayloadBuffer; }
bool Packet::isValid() { return this->valid; }

void Packet::setPacketType(uint8_t type) { this->packetType = type; }
uint8_t Packet::getPacketType() { return this->packetType; }

uint8_t *Packet::getBuffer() { return this->buffer; }
uint8_t *Packet::getPayload() { return this->payload; }
uint8_t *Packet::getHash() { return this->hash; }
uint8_t *Packet::getHeader() { return this->header; }
uint8_t *Packet::getPacketHeader() { return this->packetHeader; }

void Packet::setBufferSize(uint8_t size) { this->bufferSize = size; }
void Packet::setPayloadSize(uint8_t size) { this->payloadSize = size; }
void Packet::setHashSize(uint8_t size) { this->hashSize = size; }
void Packet::setHeaderSize(uint8_t size) { this->headerSize = size; }
void Packet::setPacketHeaderSize(uint8_t size) { this->packetHeaderSize = size; }

uint8_t Packet::getBufferSize() { return this->bufferSize; }
uint8_t Packet::getPayloadSize() { return this->payloadSize; }
uint8_t Packet::getHashSize() { return this->hashSize; }
uint8_t Packet::getHeaderSize() { return this->headerSize; }
uint8_t Packet::getPacketHeaderSize() { return this->packetHeaderSize; }

uint8_t Packet::getMaxBufferSize() { return this->maxBufferSize; }
uint8_t Packet::getMaxPayloadSize() { return this->maxPayloadSize; }

int16_t Packet::getRssi() { return this->rssi; }
void Packet::setRssi(int16_t rssi) { this->rssi = rssi; }
void Packet::setTimestamp() { this->timestamp = millis(); }
uint32_t Packet::getTimestamp() { return this->timestamp; }

#endif