/* MIT License
 * Copyright (c) 2024 Klaasjan Wagenaar, Tristan Bosveld and Richard Kroesen
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

/**
 * @file main.cpp
 * @brief Sends packets on TTN using the LISSLoRa gadget.
 * Based on the original code from: https://github.com/YourproductSmarter/KISSLoRa-demo
 * This version is using ABP pesonalisation.
 * @author Klaasjan Wagenaar, Tristan Bosveld and Richard Kroesen
 * @date 2024-03-04 (4 March)
 * @version 1.0
 */
#define RELEASE 1

#include <Arduino.h>
#include <TheThingsNetwork.h>
#include <Wire.h>

#include <main.hpp>
#include <LowPowerControl.hpp>

#include <CayenneLPP.h>
#include <SparkFun_Si7021_Breakout_Library.h>
#include <KISSLoRa_sleep.h>

/* FUNCTION PROTOTYPES */
static inline void initialize();
static void initAccelerometer(void);
static void setAccelerometerRange(uint8_t range_g);
const float get_lux_value();
const int8_t getRotaryPosition();
void getAcceleration(float *x, float *y, float *z);
void message(const uint8_t *payload, size_t size, port_t port);
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan); // TTN object for LoRaWAN radio

// Cayennel LPP
#define APPLICATION_PORT_CAYENNE 99 ///< LoRaWAN port to which CayenneLPP packets shall be sent
#define LPP_PAYLOAD_MAX_SIZE 51     ///< Maximum payload size of a LoRaWAN packet

#define LPP_CH_TEMPERATURE 0     ///< CayenneLPP CHannel for Temperature
#define LPP_CH_HUMIDITY 1        ///< CayenneLPP CHannel for Humidity sensor
#define LPP_CH_LUMINOSITY 2      ///< CayenneLPP CHannel for Luminosity sensor
#define LPP_CH_ROTARYSWITCH 3    ///< CayenneLPP CHannel for Rotary switch
#define LPP_CH_ACCELEROMETER 4   ///< CayenneLPP CHannel for Accelerometer
#define LPP_CH_BOARDVCCVOLTAGE 5 ///< CayenneLPP CHannel for Processor voltage
#define LPP_CH_PRESENCE 6        ///< CayenneLPP CHannel for Alarm
#define LPP_CH_SET_INTERVAL 20   ///< CayenneLPP CHannel for setting downlink interval
#define LPP_CH_SW_RELEASE 90     ///<

#define ALARM 0x01 ///< Alarm state
#define SAFE 0x00  ///< No-alarm state

CayenneLPP lpp(LPP_PAYLOAD_MAX_SIZE); ///< Cayenne object for composing sensor message
uint8_t lppChannel{0};                ///< channel iterator

// Sensors
Weather sensor; // temperature and humidity sensor
float x, y, z; ///< Variables to hold acellerometer axis values.

static const inline uint16_t desiredSleepDuration = 60; // In seconds. 
constexpr uint8_t wdtWakeupsPerCycle = WatchdogTimer::calculateWakeups(desiredSleepDuration);

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH); 
  initialize();
}

void loop() {
    // Check if the required number of WDT intervals has passed
  if (WatchdogTimer::wdtCounter >= wdtWakeupsPerCycle) {
    WatchdogTimer::wdtCounter = 0; // Reset counter for the next cycle
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

    lpp.reset();    // reset cayenne object
    lppChannel = 0; // reset channel counter
    lpp.addTemperature(LPP_CH_TEMPERATURE, temperature);
    lpp.addRelativeHumidity(LPP_CH_HUMIDITY, humidity);
    lpp.addLuminosity(LPP_CH_LUMINOSITY, luminosity);
    lpp.addDigitalInput(LPP_CH_ROTARYSWITCH, rotaryPosition);
    lpp.addAccelerometer(LPP_CH_ACCELEROMETER, x, y, z);
    lpp.addAnalogInput(LPP_CH_BOARDVCCVOLTAGE, vdd);

    digitalWrite(LED_LORA, LOW); // switch LED_LORA LED on

    // send cayenne message on port 99
    ttn.sendBytes(lpp.getBuffer(), lpp.getSize(), APPLICATION_PORT_CAYENNE, false, SF);
    digitalWrite(LED_LORA, HIGH); // switch LED_LORA LED off
  }
  
  WatchdogTimer::enterSleep();
}

static inline void initialize() {
  loraSerial.begin(LORA_BAUD_RATE);
  
  DEBUG_SERIAL_BEGIN();

  pinMode(RGBLED_RED, OUTPUT);
  pinMode(RGBLED_GREEN, OUTPUT);
  pinMode(RGBLED_BLUE, OUTPUT);
  pinMode(LED_LORA, OUTPUT);

  pinMode(ROTARY_PIN_0, INPUT);
  pinMode(ROTARY_PIN_1, INPUT);
  pinMode(ROTARY_PIN_2, INPUT);
  pinMode(ROTARY_PIN_3, INPUT);

  // Disable pullup resistors
  digitalWrite(ROTARY_PIN_0, 0);
  digitalWrite(ROTARY_PIN_1, 0);
  digitalWrite(ROTARY_PIN_2, 0);
  digitalWrite(ROTARY_PIN_3, 0);

  sensor.begin();

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  // Switch off leds
  digitalWrite(RGBLED_RED, HIGH);
  digitalWrite(RGBLED_GREEN, HIGH);
  digitalWrite(RGBLED_BLUE, HIGH);
  digitalWrite(LED_LORA, HIGH);

  Wire.begin();
  initAccelerometer();
  setAccelerometerRange(ACC_RANGE);

  // Initialize LoRaWAN radio
  ttn.onMessage(message); // Set callback for incoming messages
  ttn.reset(true);        // Reset LoRaWAN mac and enable ADR

  DEBUG_MSG_LN("-- STATUS");
  ttn.showStatus();
  DEBUG_MSG_LN("-- JOIN");

#if defined(OTAA)
  ttn.join(appEui, appKey);
#elif defined(ABP)
  ttn.personalize(devAddr, nwkSKey, appSKey);
#if defined(SINGLE_CHANNEL)
  setSingleChannel();
#endif
#endif
}

// /* TO DO LIST 
//  - Humidity sending.
//  - Temperature sending. 
//  - luminosity sending. 
//  - Rotary Encoder sending.
//  - Acceleratation sending. 
//  - RN2483 Spanning sending. 

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

  // Set value to -1 if it's higher than 9 - this should never happen but just in case
  if (value > 9)
  {
    value = -1;
  }

  return value;
}

// Write one register to the acceleromter
void writeAccelerometer(unsigned char REG_ADDRESS, unsigned char DATA)
{
  Wire.beginTransmission(0x1D);
  Wire.write(REG_ADDRESS);
  Wire.write(DATA);
  Wire.endTransmission();
}

// Read one register from the accelerometer
const uint8_t readAccelerometer(unsigned char REG_ADDRESS)
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

// Read the acceleration from the accelerometer, very memory intensive why....
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