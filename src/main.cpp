/* This code is free software:
 * you can redistribute it and/or modify it under the terms of a Creative
 * Commons Attribution-NonCommercial 4.0 International License
 * (http://creativecommons.org/licenses/by-nc/4.0/)
 *
 * Copyright (c) 2024 March by Klaasjan Wagenaar, Tristan Bosveld and Richard Kroesen
 */

/**
 * @file main.cpp
 * @brief Sends packets on TTN using the LISSLoRa gadget.
 * Based on the original code from: https://github.com/YourproductSmarter/KISSLoRa-demo (Refactored)
 * This version is using ABP pesonalisation.
 * @author Klaasjan Wagenaar, Tristan Bosveld and Richard Kroesen
 * @date 2024-03-04 (4 March)
 * @version 1.0
 */
#define RELEASE 1
#define CAYENNELPP_NEW // CAYENNELPP_CLASSIC

#include <Arduino.h>
#include <main.hpp>

#ifdef CAYENNELPP_CLASSIC 
  #include <CayenneLPP.h> // Library
  CayenneLPP lpp(51); ///< Cayenne object for composing sensor message
#endif
#ifdef CAYENNELPP_NEW
  #include <CayenneLPP.hpp> // Refactored Library
  PAYLOAD_ENCODER::CayenneLPP<52> lpp(51); ///< Cayenne object for composing sensor message
#endif


void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH); 
  initialize();
}

void loop() {
    DEBUG_MSG_LN("-- LOOP");

    // Measure Relative Humidity from the Si7021
    const float humidity = sensor.getRH();
    DEBUG_MSG("Humidity: ");
    DEBUG_MSG(humidity);
    DEBUG_MSG_LN(" %RH.");

    // Measure Temperature from the Si7021
    const float temperature = sensor.getTemp();
    DEBUG_MSG("Temperature: ");
    DEBUG_MSG(temperature);
    DEBUG_MSG_LN(" Degrees.");

    // Measure luminosity
    const float luminosity = get_lux_value();
    DEBUG_MSG("Ambient light: ");
    DEBUG_MSG(luminosity);
    DEBUG_MSG_LN(" lux");

    // get rotary encode position
    const uint8_t rotaryPosition = static_cast<uint8_t>(getRotaryPosition());
    DEBUG_MSG("Rotary encoder position: ");
    DEBUG_MSG_LN(rotaryPosition);

    /// get accelerometer
    float x, y, z;
    getAcceleration(&x, &y, &z);
    DEBUG_MSG("Acceleration:\tx=");
    DEBUG_MSG(x);
    DEBUG_MSG("g\n\t\ty=");
    DEBUG_MSG(y);
    DEBUG_MSG("g\n\t\tz=");
    DEBUG_MSG(z);
    DEBUG_MSG_LN("g");
    DEBUG_MSG_LN("---");

    const float vdd = (static_cast<float>(ttn.getVDD()) / 1000);
    DEBUG_MSG("RN2483 voltage: ");
    DEBUG_MSG(vdd);
    DEBUG_MSG_LN(" Volt");

#ifdef CAYENNELPP_CLASSIC
    lpp.reset();    // reset cayenne object
    lpp.addTemperature(static_cast<uint8_t>(NodeSensors::LPP_CH_TEMPERATURE), temperature);
    lpp.addRelativeHumidity(static_cast<uint8_t>(NodeSensors::LPP_CH_HUMIDITY), humidity);
    lpp.addLuminosity(static_cast<uint8_t>(NodeSensors::LPP_CH_LUMINOSITY), luminosity);
    lpp.addDigitalInput(static_cast<uint8_t>(NodeSensors::LPP_CH_ROTARYSWITCH), rotaryPosition);
    lpp.addAccelerometer(static_cast<uint8_t>(NodeSensors::LPP_CH_ACCELEROMETER), x, y, z);
    lpp.addAnalogInput(static_cast<uint8_t>(NodeSensors::LPP_CH_BOARDVCCVOLTAGE), vdd);
#endif

#ifdef CAYENNELPP_NEW
    lpp.reset();    // reset cayenne object
    lpp.addDigitalInput(static_cast<uint8_t>(NodeSensors::LPP_CH_ROTARYSWITCH), rotaryPosition);
    lpp.addTemperature(static_cast<uint8_t>(NodeSensors::LPP_CH_TEMPERATURE), temperature);
    lpp.addHumidity(static_cast<uint8_t>(NodeSensors::LPP_CH_HUMIDITY), humidity);
    lpp.addIllumination(static_cast<uint8_t>(NodeSensors::LPP_CH_LUMINOSITY), luminosity);
    lpp.addAccelerometer(static_cast<uint8_t>(NodeSensors::LPP_CH_ACCELEROMETER), x, y, z);
    lpp.addAnalogInput(static_cast<uint8_t>(NodeSensors::LPP_CH_BOARDVCCVOLTAGE), vdd);
#endif

    digitalWrite(LED_LORA, LOW); // switch LED_LORA LED on
    ttn.sendBytes(lpp.getBuffer(), lpp.getSize(), APPLICATION_FPORT_CAYENNE, false, SF);
    digitalWrite(LED_LORA, HIGH); // switch LED_LORA LED off
    delay(50000);
}

void message(const uint8_t *payload, size_t size, port_t port)
{
  DEBUG_MSG_LN("-- MESSAGE");
  DEBUG_MSG("Received " + String(size) + " bytes on port " + String(port) + ":");

  for (uint32_t i = 0; i < size; i++)
  {
    DEBUG_MSG(" " + String(payload[i]));
  }

  DEBUG_MSG_LN();

  // Toggle red LED when a message is received
  pinMode(RGBLED_RED, OUTPUT);
  digitalWrite(RGBLED_RED, !digitalRead(RGBLED_RED));
}

// Get the lux value from the APDS-9007 Ambient Light Photo Sensor
const float get_lux_value()
{
  int digital_value = analogRead(LIGHT_SENSOR_PIN);
  double vlux = digital_value * (2.56 / 1023.0); // lux value in volts
  double ilux = (vlux / 56) * 1000;              // lux value in micro amperes
  double lux = pow(10, (ilux / 10));             // Convert ilux to Lux value
  return (float)lux;                             // Return Lux value as value without decimal
}

// Poll the rotary switch 
const int8_t getRotaryPosition()
{
  int8_t value = 0;
  if (digitalRead(ROTARY_PIN_0))
  {
    value |= 1 << 0;
  }
  if (digitalRead(ROTARY_PIN_1))
  {
    value |= 1 << 1;
  }
  if (digitalRead(ROTARY_PIN_2))
  {
    value |= 1 << 2;
  }
  if (digitalRead(ROTARY_PIN_3))
  {
    value |= 1 << 3;
  }

  // Error Flow Control: Check if value is out of range boundary. 
  if (value > 9)
  {
    assert("SYS-ERR: Rotary reading failed."); 
  }
  return value;
}

// Write one register to the acceleromter
void writeAccelerometer(const uint8_t REG_ADDRESS, const uint8_t DATA)
{
  Wire.beginTransmission(0x1D);
  Wire.write(REG_ADDRESS);
  Wire.write(DATA);
  Wire.endTransmission();
}

// Read one register from the accelerometer
const uint8_t readAccelerometer(const uint8_t REG_ADDRESS)
{
  uint8_t resp;
  Wire.beginTransmission(0x1D);
  Wire.write(REG_ADDRESS);
  Wire.endTransmission(false);
  Wire.requestFrom(0x1D, 1);
  resp = Wire.read();
  return resp;
}

// Configure and activate the FXLS8471Q Accelerometer
static void initAccelerometer(void)
{
  // Check if the chip responds to the who-am-i command, should return 0x6A (106)
  if (readAccelerometer(0x0D) == 106)
  {
    // Configure FXLS8471Q CTRL_REG1 register
    // Set f_read bit to activate fast read mode
    // Set active bit to put accelerometer in active mode
    writeAccelerometer(0x2A, 0x03);
  }
  else
  {
    DEBUG_MSG_LN(F("--- I2C Accelerometer not initialized"));
  }
}

// Set the range of the FXLS8471Q Accelerometer, valid ranges: 2G,4G or 8G
static void setAccelerometerRange(uint8_t range_g)
{
  uint8_t range_b;
  switch (range_g)
  {
  case 2:
    range_b = 0;
    break;
  case 4:
    range_b = 1;
    break;
  case 8:
    range_b = 2;
    break;
  default:
    range_b = 0;
    break;
  }
  writeAccelerometer(0x0E, range_b);
}

// Read the acceleration from the accelerometer
void getAcceleration(float *x, float *y, float *z)
{
  // Resource: https://github.com/sparkfun/MMA8452_Accelerometer/blob/master/Libraries/Arduino/src/SparkFun_MMA8452Q.cpp
  // Read the acceleration from registers 1 through 6 of the MMA8452 accelerometer.
  // 2 registers per axis, 12 bits per axis.
  // Bit-shifting right does sign extension to preserve negative numbers.
  *x = ((short)(readAccelerometer(1) << 8 | readAccelerometer(2))) >> 4;
  *y = ((short)(readAccelerometer(3) << 8 | readAccelerometer(4))) >> 4;
  *z = ((short)(readAccelerometer(5) << 8 | readAccelerometer(6))) >> 4;

  // Scale 12 bit signed values to units of g. The default measurement range is ±2g.
  // That is 11 bits for positive values and 11 bits for negative values.
  // value = (value / (2^11)) * 2
  *x = (float)*x / (float)(1 << 11) * (float)(ACC_RANGE);
  *y = (float)*y / (float)(1 << 11) * (float)(ACC_RANGE);
  *z = (float)*z / (float)(1 << 11) * (float)(ACC_RANGE);
}