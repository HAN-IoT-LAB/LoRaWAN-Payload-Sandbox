# CayenneLPP Library Documentation
The CayenneLPP library provides a templated class for encoding and managing payloads compatible with the Cayenne Low Power Payload (LPP) format. It's designed for efficient data encoding for IoT devices, facilitating easy data transmission over networks like LoRaWAN.

## Features
- Templated payload encoder with customizable maximum buffer size.
- Supports various sensor types, including digital input/output, analog input/output, illumination, presence, temperature, humidity, accelerometer, barometer, gyroscope, and GPS location.
- Implements methods for adding sensor data to the payload, with automatic encoding based on sensor type.
- Provides functionality for resetting the buffer, retrieving the buffer size, and copying the buffer content.

## Getting Started

1. Including the Library.

```cpp
#include "CayenneLPP.hpp"
```

2. Initializing the Encoder
```cpp
PAYLOAD_ENCODER::CayenneLPP<51> payloadEncoder(51);
```
3. Adding Sensor Data
```cpp
// Add digital input
payloadEncoder.addDigitalInput(1, 0); // Channel 1, value 0

// Add analog input
payloadEncoder.addAnalogInput(2, 3.14f); // Channel 2, value 3.14

// Add GPS location
payloadEncoder.addGPSLocation(3, 52.3731f, 4.8922f, 10.0f); // Channel 3, lat, lon, alt
```
4. Retrieving and Using the Payload
```cpp
const uint8_t* buffer = payloadEncoder.getBuffer();
size_t bufferSize = payloadEncoder.getSize();
```
5. Send the Payload
You can use for example the TheThingsNetwork LoRaWAN radio object. 

```cpp
ttn.sendBytes(lpp.getBuffer(), lpp.getSize(), APPLICATION_FPORT_CAYENNE, false, SF);
```

5. Resetting the Buffer
```cpp
payloadEncoder.reset();
```