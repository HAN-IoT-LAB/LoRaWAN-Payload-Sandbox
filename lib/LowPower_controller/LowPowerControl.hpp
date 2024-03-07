#ifndef LOW_POWER_CONTROL_HPP
#define LOW_POWER_CONTROL_HPP 

#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

class WatchdogTimer {
public:
    static const uint8_t WDT_MAX_DURATION = 8;
    static volatile uint8_t wdtCounter;

    static constexpr uint8_t calculateWakeups(const unsigned int sleepDuration) {
        return (sleepDuration + WDT_MAX_DURATION - 1) / WDT_MAX_DURATION;
    }

    static void setupWatchdog(uint8_t timeoutSetting) {
        // Clip to 9, max value
        if (timeoutSetting > 9) {
            timeoutSetting = 9;
        }

        const uint8_t wdpSetting = timeoutSetting & 0x07;         // WDP0-2, 3 bits
        const uint8_t wdp3Setting = (timeoutSetting > 0x07) << 5; // Sets WDP3 if needed
        const uint8_t wdtControl = wdpSetting | wdp3Setting;

        cli(); // Disable interrupts temporarily
        MCUSR &= ~(1 << WDRF); // Clear the reset flag
        WDTCSR = (1 << WDCE) | (1 << WDE); // Start the timed sequence to update the WDT configuration
        WDTCSR = wdtControl | (1 << WDIE); // Set the new watchdog timeout value & enable the watchdog interrupt
        sei(); // Re-enable interrupts
    }

    static void enterSleep() {
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        ADCSRA &= ~(1 << ADEN); // Turn ADC off

        setupWatchdog(9);   // Set the watchdog to its maximum interval
        sleep_mode();       // Enter sleep mode

        // Device wakes up here when the watchdog times out
        sleep_disable();        // Disable sleep on wake up
        ADCSRA |= (1 << ADEN);  // Turn ADC on again
        power_all_enable();     // Re-enable all power
    }
};

volatile uint8_t WatchdogTimer::wdtCounter = 0;

// Watchdog Interrupt Handler
ISR(WDT_vect) {
    WatchdogTimer::wdtCounter++; // Increment the watchdog counter on each WDT interrupt.
}

#endif // LOW_POWER_CONTROL_HPP