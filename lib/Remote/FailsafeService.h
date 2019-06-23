#ifndef REMOTECONTROL_FAILSAFESERVICE_H
#define REMOTECONTROL_FAILSAFESERVICE_H

#include <Arduino.h>

class FailsafeService {
   private:
    bool fail;
    uint32_t lastUpdate;
    uint32_t delaySinceLastUpdate;

   public:
    FailsafeService();
    ~FailsafeService();
    void update();
    bool isInFailMode();
};

FailsafeService::FailsafeService() {}

FailsafeService::~FailsafeService() {}

void FailsafeService::update() { this->lastUpdate = millis(); }

bool FailsafeService::isInFailMode() { this->delaySinceLastUpdate = millis() - lastUpdate; }

#endif