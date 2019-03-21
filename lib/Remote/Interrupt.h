#ifndef REMOTECONTROL_INTERRUPT_H
#define REMOTECONTROL_INTERRUPT_H

#include <Arduino.h>

Interrupt interrupt;

class Interrupt {
   private:
   public:
    Interrupt();
    ~Interrupt();

    //   enum { RISING = 0x01, FALLING = 0x02, CHANGE = 0x03, ONLOW = 0x04, ONHIGH = 0x05, ONLOW_WE = 0x0C, ONHIGH_WE = 0x0D };

    virtual void handleInterrupt();
    static void handleThisInterrupt();
    void attach(int pin, int mode = RISING);
};

Interrupt::Interrupt() {}

Interrupt::~Interrupt() {}

void Interrupt::handleThisInterrupt() { interrupt.handleInterrupt(); }

void Interrupt::attach(int pin, int mode) { attachInterrupt(digitalPinToInterrupt(pin), Interrupt::handleThisInterrupt, mode); }
#endif