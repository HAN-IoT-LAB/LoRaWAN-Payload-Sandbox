/* This code is free software:
 * you can redistribute it and/or modify it under the terms of a Creative
 * Commons Attribution-NonCommercial 4.0 International License
 * (http://creativecommons.org/licenses/by-nc/4.0/)
 *
 * Copyright (c) 2024 March by Klaasjan Wagenaar, Tristan Bosveld and Richard Kroesen
 */

#include <CayenneLPP.hpp>

#define MAX_PAYLOAD_SIZE 51
#define SERIAL_BAUD_RATE 9600

PAYLOAD_ENCODER::CayenneLPP<MAX_PAYLOAD_SIZE> lpp(MAX_PAYLOAD_SIZE); // Initialize with max size.

/* FUNCTION PROTOTYPES: */
void decodePayload(const uint8_t* payload, uint8_t size);

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);

  // Wait for serial port to connect.
  while (!Serial) {}

  // Reset the payload before starting
  lpp.reset();
  uint8_t channel;

  // Adding a temperature reading.
  const float converted_sensor_temperature = 26.56988;
  channel = 1;
  lpp.addTemperature(channel, converted_sensor_temperature);

  // Adding a GPS location.
  float latitude = 52.3731f;
  float longitude = 4.8922f;
  float altitude = 12.0f;
  channel = 2; // Channel 2 for GPS
  lpp.addGPSLocation(channel, latitude, longitude, altitude);

  // Copy the payload into a byte array for transmission.
  const uint8_t payloadSize = lpp.getSize();
  uint8_t payload[payloadSize] = {};
  lpp.copy(payload);

  // For demonstration we just print the buffer.
  // Typically here you would send the payload.
  Serial.print("Payload Size: ");
  Serial.println(payloadSize);
  Serial.print("Payload: ");
  for (int i = 0; i < payloadSize; i++) {
    Serial.print(payload[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  decodePayload(payload, payloadSize);
}

void loop() {
  // Nothing to do here
}

void decodePayload(const uint8_t* payload, uint8_t size) {
  using namespace PAYLOAD_ENCODER;
  Serial.println("Decoding Payload:");
  size_t i = 0;
  while (i < size) {
    DATA_TYPES dataType = static_cast<DATA_TYPES>(payload[i]);
    uint8_t channel = payload[i + 1];
    i += 2; // Move past the data type and channel bytes.

    switch (dataType) {
      case DATA_TYPES::TEMP_SENS: {
        int16_t tempRaw = (payload[i+1] << 8) | payload[i];
        float temperature = tempRaw / FLOATING_DATA_RESOLUTION(DATA_TYPES::TEMP_SENS);
        Serial.print("Temperature Channel ");
        Serial.print(channel);
        Serial.print(" fixedpoint: ");
        Serial.println(temperature);
        i += 2; // Move past the temperature data.
        break;
      }
      case DATA_TYPES::GPS_LOC: {
        int32_t latRaw = (static_cast<int32_t>(payload[i+3]) << 24 | static_cast<int32_t>(payload[i + 2]) << 16 | static_cast<int32_t>(payload[i + 1]) << 8 | (payload[i]));
        i += 4;
        int32_t lonRaw = (static_cast<int32_t>(payload[i+3]) << 24 | static_cast<int32_t>(payload[i + 2]) << 16 | static_cast<int32_t>(payload[i + 1]) << 8 | (payload[i]));
        i += 4;
        int32_t altRaw = (static_cast<int32_t>(payload[i+3]) << 24 | static_cast<int32_t>(payload[i + 2]) << 16 | static_cast<int32_t>(payload[i + 1]) << 8 | (payload[i]));
        i += 4;

        float lat = latRaw / static_cast<float>(FLOATING_DATA_RESOLUTION(DATA_TYPES::GPS_LOC));     // Applying scaling factor for latitude
        float lon = lonRaw / static_cast<float>(FLOATING_DATA_RESOLUTION(DATA_TYPES::GPS_LOC));     // Applying scaling factor for longitude
        float alt = altRaw / static_cast<float>(FLOATING_DATA_RESOLUTION(DATA_TYPES::GPS_LOC))*100.0f; // Applying scaling factor for altitude has .01 precision. 

        Serial.print("GPS Channel ");
        Serial.print(channel);
        Serial.print(": Lat ");
        Serial.print(lat, 6); // More decimal places for precision
        Serial.print(", Lon ");
        Serial.print(lon, 6); // More decimal places for precision
        Serial.print(", Alt ");
        Serial.println(alt);
        break;
      }
      default:
        Serial.println("Unknown data type");
        return; // Exit the loop for unknown data type
    }
  }
}

