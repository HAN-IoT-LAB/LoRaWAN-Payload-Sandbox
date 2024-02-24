#include <Arduino.h>
#include "../lib/CayenneLPP.hpp"

PAYLOAD_ENCODER::CayenneLPP<100> payloadBuf(50);

void setup() {
  payloadBuf.addDigitalInput(1,5);
}

void loop() {
}