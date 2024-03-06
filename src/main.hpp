/* DEVICE CONFIGURATION */
#define loraSerial      Serial1
#define debugSerial     Serial

#define freqPlan        TTN_FP_EU868 // The KISS device should only be used in Europe
#define OTAA                         // ABP

#define SF 9

/* END OF DEVICE CONFIGURATION */
#define LORA_BAUD_RATE 57600
/* DEBUG CONFIG */
#define DEBUG_BAUD_RATE 9600
#define DEBUG 1

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