# LoRaWAN Payload Encoder-Decoder: CayenneLPP Extreme

Welcome to the implementation repository for HAN IoT-LAB of 2324 (semester 6) Embedded Systems Engineering. The IoT-LAB is within context of LoRaWAN as a educational vechicle for introduction to the realm of IoT applicational dev. 

 **LoRaWAN Payload Encoder Library**, This repository contains a header-only Arduino library that transcends the original CayenneLPP framework, tailored for memory footprint optimalization for projects nestled within the realms of LoRaWAN and The Things Network (TTN).

This repo is an experimental environment, so a sandbox, and also contains demonstrational embedded deployment program in the src folder. Which is heavily refactored because it was not practical for even simple experimentation. 

## Repo Structure:

- **src Folder**: Contains demonstration programs for embedded deployment, which have been refactored for enhanced usability and experimentation. [original reference](https://gitlab.com/wlgrw/han-iot-kiss-lora/-/tree/master/LoRa_TX_RX_Cayenne_HAN?ref_type=heads)
- **lib Folder**: Includes external libraries or those provided by the course.
- **include Folder**: Houses the developed library for this course.

## Features

- **Header-Only Arduino Library**: Simplifies integration and streamlines your workflow, ensuring a seamless development experience.
- **Optimized for Efficiency**: Reduces payload size significantly, offering a more resource-efficient solution that's 400 bytes leaner than the original.
- **Broad Sensor Support**: From environmental monitoring to motion detection, this library decodes a wide array of sensor data, paving the way for versatile IoT applications.
- **Precision and Compatibility**: Meticulously crafted to ensure high precision and compatibility across different platforms and devices within the LoRaWAN ecosystem.

## Getting Started
Explore this libary yourself by copying the headers: 
[CayenneLPP.hpp](./include/CayenneLPP.hpp) and [CayenneReferences.hpp](./include/CayenneReferences.hpp) files.

Or get the Arduino zipped library in the newest releases. 

## Demonstration Device
The KISS node is used for payload encoding. 
![HAN-IoT KISS LORA NODE](images/han-iot-kiss-node-example.png)
*Image by Remko Welling from* [*this repository*](https://gitlab.com/wlgrw/han-iot-kiss-lora/-/blob/master/README.md?ref_type=heads).

## Copyright Notice
The CayenneLPP Extreme library is free: You can redistribute it and/or modify it under the terms of a Creative Commons Attribution-NonCommercial 4.0 International License (http://creativecommons.org/licenses/by-nc/4.0/) by Richard Kroesen, Klaasjan Wagenaar and Tristan Bosveld.

<a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.