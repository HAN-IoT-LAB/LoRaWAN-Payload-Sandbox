/* This code is free software:
 * you can redistribute it and/or modify it under the terms of a Creative
 * Commons Attribution-NonCommercial 4.0 International License
 * (http://creativecommons.org/licenses/by-nc/4.0/)
 *
 * Copyright (c) 2024 March by Klaasjan Wagenaar, Tristan Bosveld and Richard Kroesen
 */

#ifndef MAIN_HPP
#define MAIN_HPP

#include <assert.h>
#include <Arduino.h>
#include <TheThingsNetwork.h>
#include <Wire.h>

#include <main.hpp>
#include <CayenneLPP.h>
#include <SparkFun_Si7021_Breakout_Library.h>

#include "../../include/CayenneLPP.hpp"

/* DEVICE CONFIGURATION */
#define loraSerial      Serial1
#define debugSerial     Serial

#define freqPlan        TTN_FP_EU868 // The KISS device should only be used in Europe
#define OTAA                         // ABP

#define SF 9                        // Spreading Factor. 

/* END OF DEVICE CONFIGURATION */
#define LORA_BAUD_RATE 57600
/* DEBUG CONFIG */
#define DEBUG_BAUD_RATE 9600
#define DEBUG 0

#if DEBUG
#define DEBUG_SERIAL_BEGIN() debugSerial.begin(DEBUG_BAUD_RATE)
#define DEBUG_MSG(x) debugSerial.print(x)
#define DEBUG_MSG_LN(x) debugSerial.println(x)
#else
#define DEBUG_SERIAL_BEGIN()
#define DEBUG_MSG(x)
#define DEBUG_MSG_LN(x)
#endif

/* END OF DEBUG CONFIG */
/* PIN DEFINES */

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

#define LIGHT_SENSOR_PIN 10     // Define for Analog inpu pin
#define BUTTON_PIN 7            // defines for pushbutton 
#define ACC_RANGE 2             // Set up to read the accelerometer values in range -2g to +2g - valid ranges: ±2G,±4G or ±8G

/* END OF PIN DEFINES */

#define APPLICATION_FPORT_CAYENNE 1 ///< LoRaWAN port to which CayenneLPP packets shall be sent

#if defined(OTAA)
// HAN KISS-xx: devEui is device specific
const char *appEui = "0000000000000000";
const char *appKey = "107D4313B85D35A3790A9ECDBA8F124E";
#elif defined(ABP)
const char *devAddr = "00000000";
const char *nwkSKey = "0000000000000000";
const char *appSKey = "00000000000000000000000000000000";
#else
#error "No device configured."
#endif

/* GLOBAL VARIABLES: */
Weather sensor; // temperature and humidity sensor

/* FUNCTION PROTOTYPES */
static inline void initialize();
static void initAccelerometer(void);
static void setAccelerometerRange(uint8_t range_g);
const float get_lux_value();
const int8_t getRotaryPosition();
void getAcceleration(float *x, float *y, float *z);
void message(const uint8_t *payload, size_t size, port_t port);
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan); // TTN object for LoRaWAN radio

enum class NodeSensors : uint8_t {
  LPP_CH_TEMPERATURE        = 0,    ///< CayenneLPP CHannel for Temperature
  LPP_CH_HUMIDITY           = 1,    ///< CayenneLPP CHannel for Humidity sensor
  LPP_CH_LUMINOSITY         = 2,    ///< CayenneLPP CHannel for Luminosity sensor
  LPP_CH_ROTARYSWITCH       = 3,    ///< CayenneLPP CHannel for Rotary switch
  LPP_CH_ACCELEROMETER      = 4,    ///< CayenneLPP CHannel for Accelerometer
  LPP_CH_BOARDVCCVOLTAGE    = 5,    ///< CayenneLPP CHannel for Processor voltage
  LPP_CH_PRESENCE           = 6,    ///< CayenneLPP CHannel for Alarm
};

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

#endif // MAIN_HPP