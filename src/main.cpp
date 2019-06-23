#include <Arduino.h>
#include <Remote.h>

Remote* remote = new Remote();

void setup() { Serial.begin(9600); }

void loop() { remote->receive(); }
