#include <Arduino.h>
#include <Remote.h>

Remote remote;

void setup() {
    Serial.begin(9600);
    if (!remote.initSuccsess()) {
        Serial.println("remote setup failed. check keySize!");
    }
}

void loop() {
    remote.receive();
    if (Serial.available()) {
        String s = Serial.readString();
        ExamplePayload payload;
        payload.setString(s);
        Serial.println("sent: " + s + " in " + String(remote.send(&payload)) + " millisecounds");
    }
}