/* MIT License
 * Copyright (c) 2024 Richard Kroesen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <unity.h>
#include "../include/CayenneLPP.hpp"

void setUp(void)
{
}

void tearDown(void)
{
}

// Test adding GPS data to the CayenneLPP instance
void test_addFieldImpl_GPSData(void) {
    PAYLOAD_ENCODER::CayenneLPP<250> lpp(250); // Initialize with a buffer size

    // Example sensor channel and data type for GPS location
    uint8_t sensorChannel = 1; 
    PAYLOAD_ENCODER::DATA_TYPES dataType = PAYLOAD_ENCODER::DATA_TYPES::GPS_LOC;

    // Adding GPS data with predefined values
    float lat = 51.5074f, lon = -0.1278f, alt = 30.0f;
    lpp.addField(dataType, sensorChannel, lat, lon, alt);

    uint8_t* buffer = lpp.getBuffer();

    // Static expected scaled values
    int32_t expectedScaledLat = 515074;
    int32_t expectedScaledLon = -1278;
    int32_t expectedScaledAlt = 3000;

    // Decode the stored values from the buffer
    int32_t storedScaledLat, storedScaledLon, storedScaledAlt;
    memcpy(&storedScaledLat, buffer + 2, sizeof(storedScaledLat)); 
    memcpy(&storedScaledLon, buffer + 2 + sizeof(storedScaledLat), sizeof(storedScaledLon));
    memcpy(&storedScaledAlt, buffer + 2 + sizeof(storedScaledLat) + sizeof(storedScaledLon), sizeof(storedScaledAlt));

    // Verify the stored values match the expected static values
    TEST_ASSERT_EQUAL_INT32(expectedScaledLat, storedScaledLat);
    TEST_ASSERT_EQUAL_INT32(expectedScaledLon, storedScaledLon);
    TEST_ASSERT_EQUAL_INT32(expectedScaledAlt, storedScaledAlt);
}

void test_addFieldImpl_GPSData_highPrecision(void) {
    PAYLOAD_ENCODER::CayenneLPP<250> lpp(250); // Initialize with a buffer size

    // Example sensor channel and data type for GPS location
    uint8_t sensorChannel = 1; 
    PAYLOAD_ENCODER::DATA_TYPES dataType = PAYLOAD_ENCODER::DATA_TYPES::GPS_LOC;

    // Adding GPS data with predefined values
    float lat = 51.5074111f, lon = -0.1278511f, alt = 30.0111f;
    lpp.addField(dataType, sensorChannel, lat, lon, alt);

    uint8_t* buffer = lpp.getBuffer();

    // Static expected scaled values
    int32_t expectedScaledLat = 515074;
    int32_t expectedScaledLon = -1279;
    int32_t expectedScaledAlt = 3001;

    // Decode the stored values from the buffer
    int32_t storedScaledLat, storedScaledLon, storedScaledAlt;
    memcpy(&storedScaledLat, buffer + 2, sizeof(storedScaledLat)); 
    memcpy(&storedScaledLon, buffer + 2 + sizeof(storedScaledLat), sizeof(storedScaledLon));
    memcpy(&storedScaledAlt, buffer + 2 + sizeof(storedScaledLat) + sizeof(storedScaledLon), sizeof(storedScaledAlt));

    // Verify the stored values match the expected static values
    TEST_ASSERT_EQUAL_INT32(expectedScaledLat, storedScaledLat);
    TEST_ASSERT_EQUAL_INT32(expectedScaledLon, storedScaledLon);
    TEST_ASSERT_EQUAL_INT32(expectedScaledAlt, storedScaledAlt);
}

void test_addAccelerometer(void) {
    PAYLOAD_ENCODER::CayenneLPP<250> lpp(250); // Initialize with a buffer size
    PAYLOAD_ENCODER::DATA_TYPES dataType = PAYLOAD_ENCODER::DATA_TYPES::ACCRM_SENS;

    uint8_t sensorChannel = 1; // Example sensor channel
    float x = 1.2306f, y = -2.34f, z = 3.45f;

    // Adding accelerometer data
    uint8_t result = lpp.addField(dataType, sensorChannel, x, y, z);

    // Ensure data was added successfully
    TEST_ASSERT_NOT_EQUAL(0, result);

    uint8_t* buffer = lpp.getBuffer();

    // Hardcoded expected scaled values
    int16_t expectedScaledX = 1231;
    int16_t expectedScaledY = -2340;
    int16_t expectedScaledZ = 3450;

    // Decode the stored values from the buffer for comparison
    int16_t storedScaledX, storedScaledY, storedScaledZ;

    memcpy(&storedScaledX, buffer + 2, sizeof(storedScaledX)); 
    memcpy(&storedScaledY, buffer + 2 + sizeof(storedScaledX), sizeof(storedScaledY));
    memcpy(&storedScaledZ, buffer + 2 + sizeof(storedScaledX) + sizeof(storedScaledY), sizeof(storedScaledZ));

    // Verify the stored values match the hardcoded expected scaled values
    TEST_ASSERT_EQUAL_INT16(expectedScaledX, storedScaledX);
    TEST_ASSERT_EQUAL_INT16(expectedScaledY, storedScaledY);
    TEST_ASSERT_EQUAL_INT16(expectedScaledZ, storedScaledZ);
}

void test_addFieldImpl_Temperature(void) {
    PAYLOAD_ENCODER::CayenneLPP<250> lpp(250); // Assuming a maximum buffer size of 250 bytes
    uint8_t sensorChannel = 1; // Example sensor channel for temperature
    float temperatureValue = 25.55f; // Example temperature value to add

    // Expected scaled value (assuming the resolution for TEMP_SENS might be 10 for 0.1°C precision)
    int16_t expectedScaledValue = 256;

    // Add temperature data
    uint8_t result = lpp.addField(PAYLOAD_ENCODER::DATA_TYPES::TEMP_SENS, sensorChannel, temperatureValue);

    // Verify that data was added to the buffer successfully
    TEST_ASSERT_NOT_EQUAL(0, result);

    // Extract the stored scaled value from the buffer for comparison
    int16_t storedScaledValue;
    memcpy(&storedScaledValue, &lpp.getBuffer()[2], sizeof(storedScaledValue));

    // Verify the stored scaled value matches the expected scaled value
    TEST_ASSERT_EQUAL_INT16(expectedScaledValue, storedScaledValue);
}


// // Test the reset function
// void test_reset(void) {
//     PAYLOAD_ENCODER::CayenneLPP<512> payload(100);
//     payload.addDigitalInput(1, 255); // Add a sample digital input to change the state
//     payload.reset(); // Reset the payload
//     TEST_ASSERT_EQUAL_UINT8(0, payload.getSize()); // Expect currentIndex to be 0 after reset
// }

// // Test the getSize function
// void test_getSize(void) {
//     PAYLOAD_ENCODER::CayenneLPP<512> payload(100);
//     TEST_ASSERT_EQUAL_UINT8(0, payload.getSize()); // Initially, size should be 0
//     payload.addDigitalInput(1, 255);
//     TEST_ASSERT_EQUAL_UINT8(3, payload.getSize()); // Size should reflect added data
// }

// // Test adding a digital input
// void test_addDigitalInput(void) {
//     PAYLOAD_ENCODER::CayenneLPP<512> payload(100);
//     uint8_t size = payload.addDigitalInput(1, 255);
//     TEST_ASSERT_EQUAL_UINT8(3, size); // Expect size to increase by the size of the digital input data
//     const uint8_t* buffer = payload.getBuffer();
//     TEST_ASSERT_EQUAL_UINT8(1, buffer[0]); // Channel
//     TEST_ASSERT_EQUAL_UINT8(DATA_TYPES::DIG_IN, buffer[1]); // Data type
//     TEST_ASSERT_EQUAL_UINT8(255, buffer[2]); // Value
// }

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_addFieldImpl_GPSData);
    RUN_TEST(test_addFieldImpl_GPSData_highPrecision);
    RUN_TEST(test_addAccelerometer);
    RUN_TEST(test_addFieldImpl_Temperature);
    UNITY_END();
    return 0;
}
