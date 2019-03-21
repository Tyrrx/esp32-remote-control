#ifndef REMOTECONTROL_PACKET_H
#define REMOTECONTROL_PACKET_H

class Packet {
   private:
    bool valid;
    uint8_t packetType;

    uint8_t bufferSize;
    uint8_t payloadSize;
    uint8_t hashSize;
    uint8_t headerSize;
    uint8_t packetHeaderSize;

    uint8_t* buffer;
    uint8_t* payload;
    uint8_t* hash;
    uint8_t* header;
    uint8_t* packetHeader;

    int16_t rssi;
    uint32_t timestamp;

   public:
    Packet(/* args */);
    ~Packet();

    bool isValid();

    void setPacketType(uint8_t type);
    uint8_t getPacketType();

    void createBuffer(uint8_t size);
    void createPayloadBuffer(uint8_t size);
    void createHashBuffer(uint8_t size);
    void createHeaderBuffer(uint8_t size);
    void createPacketHeaderBuffer(uint8_t size);

    uint8_t* getBuffer();
    uint8_t* getPayload();
    uint8_t* getHash();
    uint8_t* getHeader();
    uint8_t* getPacketHeader();

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

    // Describes the specific indexes in the packetHeaderBuffer and the size of it.
    enum {
        PAYLOAD_SIZE_ADDRESS,
        HEADER_SIZE_ADDRESS,
        TYPE_ADDRESS,
        PACKETHEADER_SIZE = 3,
    };
};

Packet::Packet(/* args */) { this->createPacketHeaderBuffer(Packet::PACKETHEADER_SIZE); }

Packet::~Packet() {
    if (this->buffer) {
        delete this->buffer;
    }
    if (this->packetHeader) {
        delete this->packetHeader;
    }
    if (this->header) {
        delete this->header;
    }
    if (this->hash) {
        delete this->hash;
    }
    if (this->payload) {
        delete this->payload;
    }
}

bool Packet::isValid() { return this->valid; }

void Packet::setPacketType(uint8_t type) { this->packetType = type; }
uint8_t Packet::getPacketType() { return this->packetType; }

void Packet::createBuffer(uint8_t size) {
    this->setBufferSize(size);
    this->buffer = new uint8_t[size];
}
void Packet::createPayloadBuffer(uint8_t size) {
    this->setPayloadSize(size);
    this->payload = new uint8_t[size];
}
void Packet::createHashBuffer(uint8_t size) {
    this->setHashSize(size);
    this->hash = new uint8_t[size];
}
void Packet::createHeaderBuffer(uint8_t size) {
    this->setHeaderSize(size);
    this->header = new uint8_t[size];
}
void Packet::createPacketHeaderBuffer(uint8_t size) {
    this->setPacketHeaderSize(size);
    this->packetHeader = new uint8_t[size];
}

uint8_t* Packet::getBuffer() { return this->buffer; }
uint8_t* Packet::getPayload() { return this->payload; }
uint8_t* Packet::getHash() { return this->hash; }
uint8_t* Packet::getHeader() { return this->header; }
uint8_t* Packet::getPacketHeader() { return this->packetHeader; }

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

int16_t Packet::getRssi() { return this->rssi; }
void Packet::setRssi(int16_t rssi) { this->rssi = rssi; }
void Packet::setTimestamp() { this->timestamp = millis(); }
uint32_t Packet::getTimestamp() { return this->timestamp; }

#endif