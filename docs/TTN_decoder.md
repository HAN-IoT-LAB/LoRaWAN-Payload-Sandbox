# TTN-Decoder Documentation

***Repository of the decoder V1***:
[v1.0 Decoder of the CayenneLPP - JavaScript TheThingsNetwork](https://github.com/HAN-IoT-LAB/TTN-Decoder)

The TTN-Decoder is a JavaScript-based implementation designed to decode payloads encoded using the Cayenne Low Power Payload (CayenneLPP) format. Developed with a focus on memory optimization and refactorization, this decoder operates within TheThingsNetwork (TTN), making it an ideal solution for a variety of web-based applications, user interfaces, or dashboards aimed at generic IoT applications.

## Features

The decoder boasts a comprehensive range of features designed to support a wide variety of sensor data, ensuring versatility for applications such as data loggers, telemetry systems, or device remote-monitoring. Key features include:

- **Wide Range of Sensor Support**: Capable of decoding data from a diverse array of sensors including digital inputs/outputs, analog inputs, illumination, presence, temperature, humidity, accelerometers, barometers, gyroscopes, and GPS location.
- **High Precision (Configurable)**: Features precision scaling to ensure accurate data interpretation, essential for GPS coordinates and altitude measurements.
- **Endian-Aware Processing**: Guarantees the correct handling of byte order, crucial for the accurate interpretation of multi-byte sensor data.
- **Comprehensive Unit Testing**: Ensures decoder reliability and quality, with tests conducted using the Jest Framework.

## Design Overview

The decoder's design revolves around a consistent payload structure, where the DataType is critical for establishing expectations regarding datasize, precision, signedness, and unit representation.

### Payload Structure

Encoded and decoded payloads adhere to a uniform structure outlined as follows:

```
[Sensor Type] [Channel] [Data Bytes...]
```

This structure ensures seamless encoding and decoding processes, with each component playing a vital role in data interpretation.

### Sensor Data Types

Each sensor data type directly correlates with the encoder's format, adhering to the IPSO Alliance Smart Objects Guidelines for standardized Object IDs. The conversion to fit the Object ID into a single byte is as shown below:

```
LPP_DATA_TYPE = IPSO_OBJECT_ID - 3200
```

A table detailing the supported sensor types, their corresponding type IDs, precision levels, signedness, and byte lengths is provided to clarify the data types recognized by the decoder.

## Getting Started

The decoder is primarily utilized on TheThingsNetwork but can also run natively for development or testing purposes.

### Prerequisites

- **NodeJS**: Required for native execution on personal devices.
- **Jest**: Necessary for running unit tests, installable via npm.
- **TheThingsNetwork Application**: Essential for deployment or testing with target devices.

## Testing

Testing is a crucial aspect of the decoder's development, ensuring accurate sensor data decoding. An overview of test scenarios is provided, highlighting the expected outcomes for each sensor type.

### Running Unit Tests

To execute the unit tests:

```bash
npm test
```

This command triggers the Jest-based tests, verifying the decoder's functionality across various scenarios.

## License

The TTN-Decoder is released under the Creative Commons Attribution-NonCommercial 4.0 International License, permitting redistribution and modification for non-commercial purposes. This initiative is a collaborative effort by Richard Kroesen, Klaasjan Wagenaar, and Tristan Bosveld.

![Creative Commons License](https://i.creativecommons.org/l/by-nc/4.0/88x31.png)

This work is licensed under a [Creative Commons Attribution-NonCommercial 4.0 International License](http://creativecommons.org/licenses/by-nc/4.0/).