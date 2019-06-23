#ifndef REMOTECONTROL_REMOTESERVICE_H
#define REMOTECONTROL_REMOTESERVICE_H

#include <Arduino.h>

class RemoteService {
   private:
    /* data */
   public:
    RemoteService(/* args */);
    ~RemoteService();

    void handleReceive(uint8_t* controlChannelValues);
};

RemoteService::RemoteService(/* args */) {}

RemoteService::~RemoteService() {}

void RemoteService::handleReceive(uint8_t* controlChannelValues) {
    uint16_t controlChannelValues16[12];
    for (uint8_t i = 0; i < 12; i++) {
        controlChannelValues16[i] = ((uint16_t)controlChannelValues[i] << 8 | (uint16_t)controlChannelValues[i + 1]);
    }
}

#endif