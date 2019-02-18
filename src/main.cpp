#include <Arduino.h>
#include <Remote.h>

Remote remote;

void setup() {
    Serial.begin(9600);
}

void loop() {
    remote.receive();
    ExamplePayload* payload = new ExamplePayload();
    payload->setPayload(222);
    Serial.println(remote.send(payload));
    delete payload;
    //Serial.println(system_get_free_heap_size());
    delay(800);
}