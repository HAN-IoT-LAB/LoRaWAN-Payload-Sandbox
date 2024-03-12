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

void setUp(void) {
    // Set up any required items before each test
}

void tearDown(void) {
    // Clean up after each test
}

void test_addDigitalInput(void) {
    PAYLOAD_ENCODER::CayenneLPP<64> lpp(64);
    uint8_t sensorChannel = 1;
    uint8_t value = 1; // Digital value to add (true)

    uint8_t result = lpp.addDigitalInput(sensorChannel, value);
    TEST_ASSERT_NOT_EQUAL(0, result);

    uint8_t* buffer = lpp.getBuffer();
    // Assuming the first byte is the sensor channel, the second is the data type, and the third is the value
    TEST_ASSERT_EQUAL_UINT8(PAYLOAD_ENCODER::DATA_TYPES::DIG_IN, buffer[1]);
    TEST_ASSERT_EQUAL_UINT8(value, buffer[2]);
}

void test_addAnalogInput(void) {
    PAYLOAD_ENCODER::CayenneLPP<64> lpp(64);
    uint8_t sensorChannel = 3;
    float value = 3.3f; // Example analog input value

    uint8_t result = lpp.addAnalogInput(sensorChannel, value);
    TEST_ASSERT_NOT_EQUAL(0, result);

    uint8_t* buffer = lpp.getBuffer();
    TEST_ASSERT_EQUAL_INT8(74, buffer[2]);
    TEST_ASSERT_EQUAL_INT8(1, buffer[3]);
}

void test_addAnalogOutput(void) {
    PAYLOAD_ENCODER::CayenneLPP<64> lpp(64);
    uint8_t sensorChannel = 4;
    float value = 2.2f; // Example analog output value

    uint8_t result = lpp.addAnalogOutput(sensorChannel, value);
    TEST_ASSERT_NOT_EQUAL(0, result);

    uint8_t* buffer = lpp.getBuffer();
    TEST_ASSERT_EQUAL_INT16(220, buffer[2]);
}

void test_addDigitalOutput(void) {
    PAYLOAD_ENCODER::CayenneLPP<64> lpp(64);
    uint8_t sensorChannel = 2;
    uint8_t value = 0; // Digital output value to add (false)

    uint8_t result = lpp.addDigitalOutput(sensorChannel, value);
    TEST_ASSERT_NOT_EQUAL(0, result);

    uint8_t* buffer = lpp.getBuffer();
    TEST_ASSERT_EQUAL_UINT8(PAYLOAD_ENCODER::DATA_TYPES::DIG_OUT, buffer[1]);
    TEST_ASSERT_EQUAL_UINT8(value, buffer[2]);
}

void test_addIllumination(void) {
    PAYLOAD_ENCODER::CayenneLPP<64> lpp(64);
    uint8_t sensorChannel = 5;
    uint16_t value = 550; // Example illumination value in lux

    uint8_t result = lpp.addIllumination(sensorChannel, value);
    TEST_ASSERT_NOT_EQUAL(0, result);

    uint8_t* buffer = lpp.getBuffer();
    TEST_ASSERT_EQUAL_UINT8(0x02, buffer[2]);
    TEST_ASSERT_EQUAL_UINT8(0x26, buffer[3]);
}

void test_addPresence(void) {
    PAYLOAD_ENCODER::CayenneLPP<64> lpp(64);
    uint8_t sensorChannel = 6;
    uint8_t value = 1; // Presence detected

    uint8_t result = lpp.addPresence(sensorChannel, value);
    TEST_ASSERT_NOT_EQUAL(0, result);

    uint8_t* buffer = lpp.getBuffer();
    TEST_ASSERT_EQUAL_UINT8(1, buffer[2]);
}

void test_addTemperature(void) {
    PAYLOAD_ENCODER::CayenneLPP<64> lpp(64);
    uint8_t sensorChannel = 1;
    float value = 25.5f; // Example temperature value in Celsius

    uint8_t result = lpp.addTemperature(sensorChannel, value);
    TEST_ASSERT_NOT_EQUAL(0, result);

    uint8_t* buffer = lpp.getBuffer();

    TEST_ASSERT_EQUAL_UINT8(255, buffer[2]);
}

void test_addHumidity(void) {
    PAYLOAD_ENCODER::CayenneLPP<64> lpp(64);
    uint8_t sensorChannel = 2;
    float value = 75.5f; // Example humidity value in percentage

    uint8_t result = lpp.addHumidity(sensorChannel, value);
    TEST_ASSERT_NOT_EQUAL(0, result);

    uint8_t* buffer = lpp.getBuffer();

    // Assuming humidity is stored as a single byte representing the percentage times 2 for 0.5% resolution
    uint16_t expectedValue = 755; // Convert to 0.5% increments
    uint16_t actualValue = (buffer[3] << 8) | buffer[2]; // Adjust index based on how the data is actually stored

    TEST_ASSERT_EQUAL_UINT16(expectedValue, actualValue);
}


void test_addAccelerometer(void) {
    PAYLOAD_ENCODER::CayenneLPP<64> lpp(64);
    uint8_t sensorChannel = 3;
    float x = 1.23f, y = -2.34f, z = 3.45f; // Example accelerometer values in G

    uint8_t result = lpp.addAccelerometer(sensorChannel, x, y, z);
    TEST_ASSERT_NOT_EQUAL(0, result);

    uint8_t* buffer = lpp.getBuffer();
    int index = 2; // Start index for data

    // Expected values after scaling and casting to int16_t
    int16_t expectedValues[] = {
        1230, // Convert G to milli-G
        -2340,
        3450
    };

    for (int i = 0; i < 3; i++) {
        // Reconstruct each 16-bit value from the buffer and compare
        int16_t actualValue = (static_cast<int16_t>(buffer[index+ 1]) << 8) | static_cast<int16_t>(buffer[index]);
        TEST_ASSERT_EQUAL_INT16(expectedValues[i], actualValue);
        index += 2; // Move to the next value
    }
}

void test_addBarometer(void) {
    PAYLOAD_ENCODER::CayenneLPP<64> lpp(64);
    uint8_t sensorChannel = 4;
    // Example barometric pressure in hPa, scaled by a factor (e.g., 10 for 0.1 hPa resolution)
    float value = 1013.25f; 

    uint8_t result = lpp.addBarometer(sensorChannel, value);
    TEST_ASSERT_NOT_EQUAL(0, result);

    uint8_t* buffer = lpp.getBuffer();

    int index = 2; // Start index for data

    // Calculate the expected value after scaling and casting to int16_t
    // This scaling factor must match the one used in your addBarometer implementation.
    int16_t expectedValue = 10133; // Assuming 0.1 hPa resolution for this example

    // Reconstruct the 16-bit value from the buffer
    int16_t actualValue = (static_cast<int16_t>(buffer[index+1]) << 8) | static_cast<int16_t>(buffer[index]);

    TEST_ASSERT_EQUAL_INT16(expectedValue, actualValue);
}

void test_addGyroscope(void) {
    PAYLOAD_ENCODER::CayenneLPP<64> lpp(64);
    uint8_t sensorChannel = 5;
    // Example gyroscope values in degrees per second
    float x = 0.123f, y = -0.234f, z = 0.345f;

    uint8_t result = lpp.addGyroscope(sensorChannel, x, y, z);
    TEST_ASSERT_NOT_EQUAL(0, result);

    uint8_t* buffer = lpp.getBuffer();

    int index = 2; // Start index for data

    // Assuming a resolution factor for the gyroscope; for example, 100 for 0.01 °/s precision
    // This must match the implementation in addGyroscope
    int16_t expectedX = 12;
    int16_t expectedY = -23;
    int16_t expectedZ = 35;

    // Extract the gyroscope values from the buffer
    int16_t actualX = (static_cast<int16_t>(buffer[index+1]) << 8) | static_cast<int16_t>(buffer[index]);
    index += 2;
    int16_t actualY = (static_cast<int16_t>(buffer[index+1]) << 8) | static_cast<int16_t>(buffer[index]);
    index += 2;
    int16_t actualZ = (static_cast<int16_t>(buffer[index+1]) << 8) | static_cast<int16_t>(buffer[index]);

    TEST_ASSERT_EQUAL_INT16(expectedX, actualX);
    TEST_ASSERT_EQUAL_INT16(expectedY, actualY);
    TEST_ASSERT_EQUAL_INT16(expectedZ, actualZ);
}

void test_addGPSLocation(void) {
    PAYLOAD_ENCODER::CayenneLPP<80> lpp(80);
    uint8_t sensorChannel = 6;
    // Example GPS coordinates
    float lat = 51.5074f, lon = -0.1278f, alt = 30.0f;

    uint8_t result = lpp.addGPSLocation(sensorChannel, lat, lon, alt);
    TEST_ASSERT_NOT_EQUAL(0, result);

    uint8_t* buffer = lpp.getBuffer();
    int index = 2; // Start index for data

    // Assuming resolution factors: 0.0001 for lat/lon and 0.01 for altitude
    int32_t expectedLat = 515074;
    int32_t expectedLon = -1278;
    int32_t expectedAlt = 3000; // Assuming altitude is in centimeters in the buffer

    // Extract each value from the buffer and compare
    int32_t storedScaledLat, storedScaledLon, storedScaledAlt;
    memcpy(&storedScaledLat, buffer + 2, sizeof(storedScaledLat)); 
    memcpy(&storedScaledLon, buffer + 2 + sizeof(storedScaledLat), sizeof(storedScaledLon));
    memcpy(&storedScaledAlt, buffer + 2 + sizeof(storedScaledLat) + sizeof(storedScaledLon), sizeof(storedScaledAlt));

    // Verify the stored values match the expected static values
    TEST_ASSERT_EQUAL_INT32(expectedScaledLat, storedScaledLat);
    TEST_ASSERT_EQUAL_INT32(expectedScaledLon, storedScaledLon);
    TEST_ASSERT_EQUAL_INT32(expectedScaledAlt, storedScaledAlt);
}

// Main function to run the tests
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_addDigitalInput);
    RUN_TEST(test_addDigitalOutput);
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
    // Add RUN_TEST for other tests here
    UNITY_END();
}