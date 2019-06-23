#ifndef REMOTECONTROL_CONTROLPAYLOADFACTORY_H
#define REMOTECONTROL_CONTROLPAYLOADFACTORY_H

#include <AbstractPayloadFactory.h>
#include <ControlPayload.h>

class ControlPayloadFactory : public AbstractPayloadFactory {
   private:
    RemoteService* remoteService;

   public:
    ControlPayloadFactory(RemoteService* remoteService);
    ~ControlPayloadFactory();

    AbstractPayload* create(uint8_t* packetHeader);
};

ControlPayloadFactory::ControlPayloadFactory(RemoteService* remoteService) { this->remoteService = remoteService; }

ControlPayloadFactory::~ControlPayloadFactory() {}

AbstractPayload* ControlPayloadFactory::create(uint8_t* packetHeader) { return new ControlPayload(this->remoteService); }

#endif