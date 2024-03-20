
In the table presented below, we offer a comprehensive overview of the unit tests conducted on the refactored CayenneLPP library. These tests are crucial for validating the functionality and reliability of the library across various scenarios. The unit tests have been meticulously crafted using the Unity Framework. Unity provides a straightforward approach to writing and running tests, making it an ideal choice for our library's validation process.

The design of our tests is heavily influenced by the AAA (Arrange, Act, Assert) methodology, a standard structure that promotes clarity and maintainability in test writing.

The focus of these tests is primarily on input and output validation, ensuring that for given inputs, the library produces expected outputs without errors. This includes testing for correct payload encoding based on sensor data, verifying the integrity of encoded data, and ensuring that the library behaves as intended across a wide range of input values.

All tests are ran natively for development convenience. 

| Test Function                  | Description                                                                | Expected Outcome                                                                                      |
|--------------------------------|----------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------|
| `test_addDigitalInput`         | Tests adding a digital input to the payload.                               | Non-zero result; buffer contains correct data type, channel, and value; size is 3.                    |
| `test_addDigitalOutput`        | Tests adding a digital output to the payload.                              | Non-zero result; buffer contains correct data type, channel, and value; value matches provided input. |
| `test_addDigitalOutput_bool`   | Tests adding a boolean digital output to the payload.                      | Non-zero result; buffer contains correct data type, channel, and boolean value as true.               |
| `test_addAnalogInput`          | Tests adding an analog input with a floating-point value to the payload.   | Non-zero result; buffer contains correct data type, channel, and scaled value; size is accurate.      |
| `test_addAnalogOutput`         | Tests adding an analog output to the payload.                              | Non-zero result; buffer contains correct data type, channel, and value as scaled integer.             |
| `test_addIllumination`         | Tests adding an illumination value to the payload.                         | Non-zero result; buffer correctly represents illumination sensor data with little endian conversion.  |
| `test_addPresence`             | Tests adding a presence detection value to the payload.                     | Non-zero result; buffer contains correct presence data.                                                |
| `test_addTemperature`          | Tests adding a temperature value to the payload.                           | Non-zero result; buffer contains correct temperature data; size matches expectation.                   |
| `test_addHumidity`             | Tests adding a humidity value to the payload.                              | Non-zero result; buffer represents humidity correctly in 0.5% increments.                              |
| `test_addAccelerometer`        | Tests adding accelerometer data (x, y, z) to the payload.                   | Non-zero result; buffer accurately represents accelerometer data; correct data size.                  |
| `test_addBarometer`            | Tests adding barometric pressure to the payload.                           | Non-zero result; buffer accurately represents barometric data; correct endianess and data size.       |
| `test_addGyroscope`            | Tests adding gyroscope data (x, y, z) to the payload.                       | Non-zero result; buffer accurately represents gyroscope data; correct data size.                      |
| `test_addGPSLocation`          | Tests adding GPS location data (lat, lon, alt) to the payload.              | Non-zero result; buffer accurately represents GPS location data; correct data size.                   |
| `test_CayenneLPP_CopyAssignment` | Tests the copy assignment operator for `CayenneLPP` objects.             | Copied object's buffer matches source; correct size.                                                   |
| `test_CopyToValidBuffer`       | Tests copying payload data to a provided buffer.                           | Copied bytes match expected number; destination buffer matches source payload.                         |
| `test_CopyToNullBuffer`        | Tests behavior when attempting to copy payload data to a `nullptr` buffer. | Copied bytes are 0; function handles `nullptr` gracefully.                                             |

## Result: PASSED