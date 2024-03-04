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

#include <CayenneLPP.h>
#include <SparkFun_Si7021_Breakout_Library.h>
#include <KISSLoRa_sleep.h>

// CONFIGURATION:
#define loraSerial Serial1
#define debugSerial Serial

#define freqPlan TTN_FP_EU868 // The KISS device should only be used in Europe
#define OTAA                  // ABP

#if defined(OTAA)
// Set your AppEUI and AppKey
// HAN KISS-xx: devEui is device specific
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";
#elif defined(ABP)
const char *devAddr = "00000000";
const char *nwkSKey = "0000000000000000";
const char *appSKey = "00000000000000000000000000000000";
#else
#error "No device configured."
#endif

/* FUNCTION PROTOTYPES */
static void initAccelerometer(void);
static void setAccelerometerRange(uint8_t range_g);
float get_lux_value();
int8_t getRotaryPosition();
void getAcceleration(float *x, float *y, float *z);
void message(const uint8_t *payload, size_t size, port_t port);

// Enable single channel operation, uncomment define to enable
// #define SINGLE_CHANNEL

// Define SF to override default (0)
// #define SF 0  ///< default SF (7)
#define SF 9

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

#define LIGHT_SENSOR_PIN 10 // Define for Analog inpu pin

// defines for LEDs
#define RGBLED_RED 12
#define RGBLED_GREEN 6
#define RGBLED_BLUE 11
#define LED_LORA 8

// defines for rotary encoder
#define ROTARY_PIN_0 5
#define ROTARY_PIN_1 13
#define ROTARY_PIN_2 9
#define ROTARY_PIN_3 30

// defines for pushbutton
#define BUTTON_PIN 7

// defines for accelerometer
// Set up to read the accelerometer values in range -2g to +2g - valid ranges: ±2G,±4G or ±8G
#define ACC_RANGE 2

float x, y, z; ///< Variables to hold acellerometer axis values.

void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);

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
  while (!debugSerial && millis() < 10000)
    ;

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

  debugSerial.println("-- STATUS");
  ttn.showStatus();
  debugSerial.println("-- JOIN");

#if defined(OTAA)
  ttn.join(appEui, appKey);
#elif defined(ABP)
  ttn.personalize(devAddr, nwkSKey, appSKey);
#if defined(SINGLE_CHANNEL)
  setSingleChannel();
#endif
#endif
}

void loop()
{
  debugSerial.println("-- LOOP");

  // Measure Relative Humidity from the Si7021
  const float humidity = sensor.getRH();
  debugSerial.print("Humidity: ");
  debugSerial.print(humidity);
  debugSerial.println(" %RH.");

  // Measure Temperature from the Si7021
  const float temperature = sensor.getTemp();
  debugSerial.print("Temperature: ");
  debugSerial.print(temperature);
  debugSerial.println(" Degrees.");

  // Measure luminosity
  const float luminosity = get_lux_value();
  Serial.print("Ambient light: ");
  Serial.print(luminosity);
  Serial.println(" lux");

  // get rotary encode position
  const uint8_t rotaryPosition = static_cast<uint8_t>(getRotaryPosition());
  Serial.print("Rotary encoder position: ");
  Serial.println(rotaryPosition);

  /// get accelerometer
  getAcceleration(&x, &y, &z);
  Serial.print("Acceleration:\tx=");
  Serial.print(x);
  Serial.print("g\n\t\ty=");
  Serial.print(y);
  Serial.print("g\n\t\tz=");
  Serial.print(z);
  Serial.println("g");
  Serial.println("---");

  const float vdd = (static_cast<float>(ttn.getVDD()) / 1000);
  Serial.print("RN2483 voltage: ");
  Serial.print(vdd);
  Serial.println(" Volt");

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
  delay(60000);                 // RK: WHY?
}

void message(const uint8_t *payload, size_t size, port_t port)
{
  debugSerial.println("-- MESSAGE");
  debugSerial.print("Received " + String(size) + " bytes on port " + String(port) + ":");

  for (uint32_t i = 0; i < size; i++)
  {
    debugSerial.print(" " + String(payload[i]));
  }

  debugSerial.println();

  // Toggle red LED when a message is received
  pinMode(RGBLED_RED, OUTPUT);
  digitalWrite(RGBLED_RED, !digitalRead(RGBLED_RED));
}

// Get the lux value from the APDS-9007 Ambient Light Photo Sensor
float get_lux_value()
{
  int digital_value = analogRead(LIGHT_SENSOR_PIN);
  double vlux = digital_value * (2.56 / 1023.0); // lux value in volts
  double ilux = (vlux / 56) * 1000;              // lux value in micro amperes
  double lux = pow(10, (ilux / 10));             // Convert ilux to Lux value
  return (float)lux;                             // Return Lux value as value without decimal
}

// Poll the rotary switch 
int8_t getRotaryPosition()
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

void buttonPressedISR()
{
  // Toggle LED when button is pressed
  // digitalWrite(LED_PIN, !digitalRead(LED_PIN));
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
uint8_t readAccelerometer(unsigned char REG_ADDRESS)
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
    Serial.println(F("--- I2C Accelerometer not initialized"));
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

//  Disable 7 of the 8 default channels. This basically disables the
//  mandatory LoRaWAN channel hopping, so FOR TESTING ONLY!!
//  Use: sendChSet(MAC_CHANNEL_STATUS, ch, "on");
//  like: exec("mac set ch status 0 off");
//  This is a bruteforce override that shall be optimized.
void setSingleChannel(void)
{
  delay(100);
  while (loraSerial.available())
  {
    loraSerial.read();
  }
  loraSerial.println("mac set ch status 1 off");
  String ret = loraSerial.readStringUntil('\n');

  delay(100);
  while (loraSerial.available())
  {
    loraSerial.read();
  }
  loraSerial.println("mac set ch status 2 off");
  ret = loraSerial.readStringUntil('\n');

  delay(100);
  while (loraSerial.available())
  {
    loraSerial.read();
  }
  loraSerial.println("mac set ch status 3 off");
  ret = loraSerial.readStringUntil('\n');

  delay(100);
  while (loraSerial.available())
  {
    loraSerial.read();
  }
  loraSerial.println("mac set ch status 4 off");
  ret = loraSerial.readStringUntil('\n');

  delay(100);
  while (loraSerial.available())
  {
    loraSerial.read();
  }
  loraSerial.println("mac set ch status 5 off");
  ret = loraSerial.readStringUntil('\n');

  delay(100);
  while (loraSerial.available())
  {
    loraSerial.read();
  }
  loraSerial.println("mac set ch status 6 off");
  ret = loraSerial.readStringUntil('\n');

  delay(100);
  while (loraSerial.available())
  {
    loraSerial.read();
  }
  loraSerial.println("mac set ch status 7 off");
  ret = loraSerial.readStringUntil('\n');

  //  ret.trim();
}
