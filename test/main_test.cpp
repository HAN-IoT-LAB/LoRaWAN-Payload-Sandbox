/* This code is free software:
 * you can redistribute it and/or modify it under the terms of a Creative
 * Commons Attribution-NonCommercial 4.0 International License
 * (http://creativecommons.org/licenses/by-nc/4.0/)
 * 
 * Copyright (c) 2024 March by Richard Kroesen
 */

#include <unity.h>
#include "../include/CayenneLPP.hpp"
#include <cstring>

#define BUF_DEFAULT 64

// Unity.h Required Defaults:
void setUp(void) {}
void tearDown(void) {}

void test_addDigitalInput(void) {
    PAYLOAD_ENCODER::CayenneLPP<BUF_DEFAULT> lpp(BUF_DEFAULT);
    uint8_t sensorChannel = 17;
    uint8_t value = 8; 

    uint8_t result = lpp.addDigitalInput(sensorChannel, value);
    const uint8_t* buffer = lpp.getBuffer();

    TEST_ASSERT_NOT_EQUAL(0, result);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(PAYLOAD_ENCODER::DATA_TYPES::DIG_IN), buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(17, buffer[1]);
    TEST_ASSERT_EQUAL_UINT8(8, buffer[2]);
    TEST_ASSERT_EQUAL_size_t(3, lpp.getSize());
}

void test_addDigitalOutput_bool(void) {
    PAYLOAD_ENCODER::CayenneLPP<BUF_DEFAULT> lpp(BUF_DEFAULT);
    uint8_t sensorChannel = 2;
    bool value = true; // Digital output value to add (false)

    uint8_t result = lpp.addDigitalOutput(sensorChannel, value);
    const uint8_t* buffer = lpp.getBuffer();
    auto flag = buffer[2];

    TEST_ASSERT_NOT_EQUAL(0, result);
    TEST_ASSERT_EQUAL_UINT8(PAYLOAD_ENCODER::DATA_TYPES::DIG_OUT, buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(sensorChannel, buffer[1]);
    TEST_ASSERT_EQUAL(true, flag);
}

void test_addDigitalOutput(void) {
    PAYLOAD_ENCODER::CayenneLPP<BUF_DEFAULT> lpp(BUF_DEFAULT);
    uint8_t sensorChannel = 2;
    uint8_t value = 50; // Digital output value to add (false)

    uint8_t result = lpp.addDigitalOutput(sensorChannel, value);
    const uint8_t* buffer = lpp.getBuffer();

    TEST_ASSERT_NOT_EQUAL(0, result);
    TEST_ASSERT_EQUAL_UINT8(PAYLOAD_ENCODER::DATA_TYPES::DIG_OUT, buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(sensorChannel, buffer[1]);
    TEST_ASSERT_EQUAL_UINT8(50, buffer[2]);
}

void test_addAnalogInput(void) {
    PAYLOAD_ENCODER::CayenneLPP<BUF_DEFAULT> lpp(BUF_DEFAULT);
    uint8_t sensorChannel = 3;
    const float value = 3.3f; // Will become 330 Real fixed Number. (2decimal precision)

    const uint8_t result = lpp.addAnalogInput(sensorChannel, value);
    const uint8_t* buffer = lpp.getBuffer();

    TEST_ASSERT_NOT_EQUAL(0, result);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(PAYLOAD_ENCODER::DATA_TYPES::ANL_IN), buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(3, buffer[1]);
    TEST_ASSERT_EQUAL_INT8(74, buffer[2]);
    TEST_ASSERT_EQUAL_INT8(1, buffer[3]);
}

void test_addAnalogOutput(void) {
    PAYLOAD_ENCODER::CayenneLPP<BUF_DEFAULT> lpp(BUF_DEFAULT);
    uint8_t sensorChannel = 4;
    float value = 2.2f;

    uint8_t result = lpp.addAnalogOutput(sensorChannel, value);
    const uint8_t* buffer = lpp.getBuffer();

    TEST_ASSERT_NOT_EQUAL(0, result);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(PAYLOAD_ENCODER::DATA_TYPES::ANL_OUT), buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(4, buffer[1]);
    TEST_ASSERT_EQUAL_INT16(220, buffer[2]); // Instaed of INT8 asserts. 
}

void test_addIllumination(void) {
    PAYLOAD_ENCODER::CayenneLPP<BUF_DEFAULT> lpp(BUF_DEFAULT);
    uint8_t sensorChannel = 5;
    uint16_t value = 550; // Example illumination value in lux

    uint8_t result = lpp.addIllumination(sensorChannel, value);
    const uint8_t* buffer = lpp.getBuffer();
   
    TEST_ASSERT_NOT_EQUAL(0, result);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(PAYLOAD_ENCODER::DATA_TYPES::ILLUM_SENS), buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(sensorChannel, buffer[1]);
    // Little Endian bitshift conversion:
    TEST_ASSERT_EQUAL_UINT8((value & 0xFF), buffer[2]);
    TEST_ASSERT_EQUAL_UINT8((value >> 8), buffer[3]);
}

void test_addPresence(void) {
    PAYLOAD_ENCODER::CayenneLPP<BUF_DEFAULT> lpp(BUF_DEFAULT);
    uint8_t sensorChannel = 66;
    uint8_t value = 1; // Presence detected

    uint8_t result = lpp.addPresence(sensorChannel, value);
    const uint8_t* buffer = lpp.getBuffer();

    TEST_ASSERT_NOT_EQUAL(0, result);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(PAYLOAD_ENCODER::DATA_TYPES::PRSNC_SENS), buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(sensorChannel, buffer[1]);
    TEST_ASSERT_EQUAL_UINT8(1, buffer[2]);
}

void test_addTemperature(void) {
    PAYLOAD_ENCODER::CayenneLPP<BUF_DEFAULT> lpp(BUF_DEFAULT);
    uint8_t sensorChannel = 102;
    float value = 25.5321f; // Example temperature value in Celsius

    uint8_t result = lpp.addTemperature(sensorChannel, value);
    const uint8_t* buffer = lpp.getBuffer();

    TEST_ASSERT_NOT_EQUAL(0, result);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(PAYLOAD_ENCODER::DATA_TYPES::TEMP_SENS), buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(sensorChannel, buffer[1]);
    TEST_ASSERT_EQUAL_UINT8(255, buffer[2]);
    TEST_ASSERT_EQUAL_UINT8(0, buffer[3]);

    TEST_ASSERT_EQUAL_UINT8(4, lpp.getSize());
}

void test_addHumidity(void) {
    PAYLOAD_ENCODER::CayenneLPP<BUF_DEFAULT> lpp(BUF_DEFAULT);
    uint8_t sensorChannel = 2;
    float value = 75.55f; // Example humidity value in percentage

    uint8_t result = lpp.addHumidity(sensorChannel, value);
    const uint8_t* buffer = lpp.getBuffer();

    uint16_t expectedValue = 756; // Convert to 0.5% increments
    uint16_t actualValue = (buffer[3] << 8) | buffer[2];

    TEST_ASSERT_NOT_EQUAL(0, result);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(PAYLOAD_ENCODER::DATA_TYPES::HUM_SENS), buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(sensorChannel, buffer[1]);
    TEST_ASSERT_EQUAL_UINT16(expectedValue, actualValue);
    TEST_ASSERT_EQUAL_UINT8(4, lpp.getSize());
}

void test_addAccelerometer(void) {
    PAYLOAD_ENCODER::CayenneLPP<BUF_DEFAULT> lpp(BUF_DEFAULT);
    uint8_t sensorChannel = 3;
    float x = 1.23f, y = -2.34f, z = 3.45f; // Example accelerometer values in G

    uint8_t result = lpp.addAccelerometer(sensorChannel, x, y, z);
    const uint8_t* buffer = lpp.getBuffer();

    int index = 2; // Start index for data

    // Expected values after scaling and casting to int16_t
    int16_t expectedValues[] = {
        1230, // Convert G to milli-G
        -2340,
        3450
    };

    for (int i = 0; i < 3; i++) {
        // Reconstruct each 16-bit value from the buffer and compare
        int16_t actualValue = (static_cast<int16_t>(buffer[index+1]) << 8) | static_cast<int16_t>(buffer[index]);
        TEST_ASSERT_EQUAL_INT16(expectedValues[i], actualValue);
        index += 2; // Move to the next value
    }

    TEST_ASSERT_NOT_EQUAL(0, result);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(PAYLOAD_ENCODER::DATA_TYPES::ACCRM_SENS), buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(3, buffer[1]);
    TEST_ASSERT_EQUAL_UINT8(PAYLOAD_ENCODER::getDataTypeSize(PAYLOAD_ENCODER::DATA_TYPES::ACCRM_SENS) + 2, lpp.getSize());
}

void test_addBarometer(void) {
    PAYLOAD_ENCODER::CayenneLPP<BUF_DEFAULT> lpp(BUF_DEFAULT);
    uint8_t sensorChannel = 4;
    float value = 1013.25f; 

    uint8_t result = lpp.addBarometer(sensorChannel, value);
    const uint8_t* buffer = lpp.getBuffer();

    int index = 2;
    int16_t expectedValue = 10133;

    // Reconstruct the 16-bit value from the buffer
    int16_t actualValue = (static_cast<int16_t>(buffer[index+1]) << 8) | static_cast<int16_t>(buffer[index]);

    TEST_ASSERT_NOT_EQUAL(0, result);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(PAYLOAD_ENCODER::DATA_TYPES::BARO_SENS), buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(sensorChannel, buffer[1]);
    TEST_ASSERT_EQUAL_INT16(expectedValue, actualValue);
    TEST_ASSERT_EQUAL_UINT8(PAYLOAD_ENCODER::getDataTypeSize(PAYLOAD_ENCODER::DATA_TYPES::BARO_SENS) + 2, lpp.getSize());
}

void test_addGyroscope(void) {
    PAYLOAD_ENCODER::CayenneLPP<BUF_DEFAULT> lpp(BUF_DEFAULT);
    const uint8_t sensorChannel = 5;
    // Example gyroscope values in degrees per second
    float x = 0.123f, y = -0.234f, z = 0.345f;

    uint8_t result = lpp.addGyroscope(sensorChannel, x, y, z);
    const uint8_t* buffer = lpp.getBuffer();
    int index = 2; // Start index for data

    int16_t expectedX = 12;
    int16_t expectedY = -23;
    int16_t expectedZ = 35;

    // Extract the gyroscope values from the buffer
    int16_t actualX = (static_cast<int16_t>(buffer[index+1]) << 8) | static_cast<int16_t>(buffer[index]);
    index += 2;
    int16_t actualY = (static_cast<int16_t>(buffer[index+1]) << 8) | static_cast<int16_t>(buffer[index]);
    index += 2;
    int16_t actualZ = (static_cast<int16_t>(buffer[index+1]) << 8) | static_cast<int16_t>(buffer[index]);

    TEST_ASSERT_NOT_EQUAL(0, result);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(PAYLOAD_ENCODER::DATA_TYPES::GYRO_SENS), buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(sensorChannel, buffer[1]);
    TEST_ASSERT_EQUAL_INT16(expectedX, actualX);
    TEST_ASSERT_EQUAL_INT16(expectedY, actualY);
    TEST_ASSERT_EQUAL_INT16(expectedZ, actualZ);
    TEST_ASSERT_EQUAL_UINT8(PAYLOAD_ENCODER::getDataTypeSize(PAYLOAD_ENCODER::DATA_TYPES::GYRO_SENS) + 2, lpp.getSize());
}

void test_addGPSLocation(void) {
    PAYLOAD_ENCODER::CayenneLPP<BUF_DEFAULT> lpp(BUF_DEFAULT);
    uint8_t sensorChannel = 6;
    // Example GPS coordinates
    float lat = 51.5074f, lon = -0.1278f, alt = 30.0f;

    uint8_t result = lpp.addGPSLocation(sensorChannel, lat, lon, alt);
    const uint8_t* buffer = lpp.getBuffer();
    int index = 2; // Start index for data

    // resolution factors: 0.0001 for lat/lon and 0.01 for altitude
    int32_t expectedLat = 515074;
    int32_t expectedLon = -1278;
    int32_t expectedAlt = 3000;

    // Extract each value from the buffer and compare
    int32_t storedScaledLat, storedScaledLon, storedScaledAlt;
    memcpy(&storedScaledLat, buffer + 2, sizeof(storedScaledLat)); 
    memcpy(&storedScaledLon, buffer + 2 + sizeof(storedScaledLat), sizeof(storedScaledLon));
    memcpy(&storedScaledAlt, buffer + 2 + sizeof(storedScaledLat) + sizeof(storedScaledLon), sizeof(storedScaledAlt));

    TEST_ASSERT_NOT_EQUAL(0, result);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(PAYLOAD_ENCODER::DATA_TYPES::GPS_LOC), buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(sensorChannel, buffer[1]);
    TEST_ASSERT_EQUAL_INT32(expectedLat, storedScaledLat);
    TEST_ASSERT_EQUAL_INT32(expectedLon, storedScaledLon);
    TEST_ASSERT_EQUAL_INT32(expectedAlt, storedScaledAlt);
    TEST_ASSERT_EQUAL_UINT8(PAYLOAD_ENCODER::getDataTypeSize(PAYLOAD_ENCODER::DATA_TYPES::GPS_LOC) + 2, lpp.getSize());
}

void test_CayenneLPP_CopyAssignment(void) {
    PAYLOAD_ENCODER::CayenneLPP<128> source(128);
    uint8_t sensorChannel = 6;
    float lat = 51.5074f, lon = -0.1278f, alt = 30.0f;

    source.addTemperature(1, 25.5);
    source.addGPSLocation(sensorChannel, lat, lon, alt);
    
    PAYLOAD_ENCODER::CayenneLPP<128> target(128);
    
    target = source;  
    const uint8_t* sourceBuffer = source.getBuffer();
    const uint8_t* targetBuffer = target.getBuffer();

    TEST_ASSERT_EQUAL(source.getSize(), target.getSize()); 
    for (size_t i = 0; i < source.getSize(); i++) {
        TEST_ASSERT_EQUAL_UINT8(sourceBuffer[i], targetBuffer[i]);
    }
}

void test_CopyToValidBuffer(void) {
    PAYLOAD_ENCODER::CayenneLPP<128> lpp(128);
    lpp.addTemperature(1, 25.5);
    uint8_t destBuffer[128];
    uint8_t expectedBytes = lpp.getSize();

    uint8_t copiedBytes = lpp.copy(destBuffer); 

    TEST_ASSERT_EQUAL_UINT8(expectedBytes, copiedBytes);
    for (uint8_t i = 0; i < copiedBytes; i++) {
        TEST_ASSERT_EQUAL_UINT8(lpp.getBuffer()[i], destBuffer[i]);
    }
}

void test_CopyToNullBuffer(void) {
    PAYLOAD_ENCODER::CayenneLPP<128> lpp(128);
    lpp.addHumidity(2, 75.0);

    uint8_t copiedBytes = lpp.copy(nullptr);

    TEST_ASSERT_EQUAL_UINT8(0, copiedBytes);
}

// Main function
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_addDigitalInput);
    RUN_TEST(test_addDigitalOutput);
    RUN_TEST(test_addDigitalOutput_bool);
    RUN_TEST(test_addAnalogInput);
    RUN_TEST(test_addAnalogOutput); 
    RUN_TEST(test_addIllumination);
    RUN_TEST(test_addPresence);
    RUN_TEST(test_addTemperature);
    RUN_TEST(test_addHumidity);
    RUN_TEST(test_addAccelerometer);
    RUN_TEST(test_addBarometer);
    RUN_TEST(test_addGyroscope);
    RUN_TEST(test_addGPSLocation);
    RUN_TEST(test_CayenneLPP_CopyAssignment);
    RUN_TEST(test_CopyToValidBuffer);
    RUN_TEST(test_CopyToNullBuffer);
    UNITY_END();
}