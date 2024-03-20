# Home

## Introduction to the Payload Encoder-Decoder Library Project

The Payload Encoder-Decoder Library project is an initiative by HAN University of Applied Sciences, aimed at developing a lean and efficient library for encoding and decoding payloads for IoT devices using LoRaWAN. This project seeks to address the limitations of the CayenneLPP library by minimizing the code footprint and maintaining specified compatibility, thereby facilitating the addition of more complex nodes to IoT devices like the HAN-KISS-LoRa and HAN-IoT-node.

The purpose of this project is to get a typical IoT development environment, encounter IoT related challenges like strained memory useage and LoRaWAN coverage. 

## Project Assignments Overview

### Assignment 1: Library Refactoring
The first assignment involves refactoring the CayenneLPP library to reduce its code footprint significantly. The refactored library is expected to support basic functionalities such as adding various data types to a payload and including a version number or type at the beginning of the payload.

### Assignment 2: Payload Decoder Implementation
The second assignment requires the implementation of a payload decoder compatible with the new encoder. This decoder should adhere to the LoRaWAN Payload API specifications, enabling seamless integration with LoRaWAN Network Servers (LNS).

### Assignment 3: Arduino Library Publication
The third assignment focuses on publishing the new library as an Arduino Library, adhering to Arduino Library specifications. This includes comprehensive documentation and example sketches to aid adoption and implementation.

### Assignment 4: Knowledge Sharing
The final assignment emphasizes the importance of knowledge transfer, requiring a presentation of the project work during a technical meeting. This involves demonstrating the library's operation, discussing key engineering decisions, and fielding questions about the implementation.

## Architecture Principles

The project is guided by several architecture principles, including the use of the C++11 standard for coding, adherence to version management best practices with Git, licensing under Creative Commons Attribution 4.0 International License (CC BY 4.0), comprehensive testing with unit tests, and detailed documentation using Doxygen.

## Getting Started with the Library

The Payload Encoder-Decoder Library is designed for simplicity and efficiency, enabling IoT developers to easily incorporate it into their projects. Here are the first steps to get started:

- **Include the Library**: Ensure the library is included in your project.
- **Initialize the Encoder/Decoder**: Create an instance of the encoder or decoder with the desired specifications.
- **Add Data to Payload**: Use the library's functions to add various types of data to your payload.
- **Encode/Decode Payloads**: Leverage the library's capabilities to encode or decode payloads as needed for your IoT application.

For more detailed information on using the library, including code examples and configuration options, please refer to the subsequent pages of this documentation.
