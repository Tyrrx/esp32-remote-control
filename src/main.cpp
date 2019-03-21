#include <Arduino.h>
#include <Remote.h>

void setup() { Serial.begin(9600); }

void loop() {
    Remote* remote = new Remote();
    remote->receive();
    if (Serial.available()) {
        String s = Serial.readString();
        ExamplePayload payload;
        payload.setString(s);
        Serial.println("sent: " + s + " in " + String(remote->send(&payload)) + " millisecounds");
    }
    attachInterrupt(digitalPinToInterrupt(2), remote->rec, RISING);
}