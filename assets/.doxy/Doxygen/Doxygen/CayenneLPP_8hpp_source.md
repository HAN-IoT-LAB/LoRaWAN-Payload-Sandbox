

# File CayenneLPP.hpp

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**CayenneLPP.hpp**](CayenneLPP_8hpp.md)

[Go to the documentation of this file](CayenneLPP_8hpp.md)

```C++

/* This code is free software:
 * you can redistribute it and/or modify it under the terms of a Creative
 * Commons Attribution-NonCommercial 4.0 International License
 * (http://creativecommons.org/licenses/by-nc/4.0/)
 *
 * Copyright (c) 2024 March by Klaasjan Wagenaar, Tristan Bosveld and Richard Kroesen
 */

#ifndef CAYENNE_LPP_HPP
#define CAYENNE_LPP_HPP

#include <stdint.h>
#include "CayenneReferences.hpp"

namespace PAYLOAD_ENCODER
{
    template <size_t MaxSize>
    class CayenneLPP
    {
    public:
        explicit CayenneLPP(const uint8_t size) : operationalSize(size > MaxSize ? MaxSize : size), currentIndex(0) 
        {
            for(size_t i = 0; i < MaxSize; i++) {
                buffer[i] = 0;
            }
        }

        CayenneLPP(const CayenneLPP& other) : operationalSize(other.operationalSize), currentIndex(other.currentIndex)
        {
            for(size_t i = 0; i < currentIndex; ++i)
            {
                buffer[i] = other.buffer[i];
            }
        }

        CayenneLPP& operator=(const CayenneLPP& other) {
            if (this != &other) {
                currentIndex = other.currentIndex;
                memcpyAVR(buffer, other.getBuffer(), currentIndex);
            }
            return *this;
        }

        ~CayenneLPP() {}

        /* REQUIRED FUNCTIONS by ASSIGNMENT #1 */
        void reset()
        {
            currentIndex = 0; // Reset currentIndex when buffer is reset
        }

        size_t getSize(void) const
        {
            return currentIndex; // Returns the count of used bytes
        }

        const uint8_t *getBuffer(void) const
        {
            return buffer;
        }

        const uint8_t copy(uint8_t *destBuffer) const
        {
            if (!destBuffer)
            {
                return 0; // Safety check to ensure the destination buffer is valid
            }
            memcpyAVR(destBuffer, buffer, currentIndex);
            return static_cast<uint8_t>(currentIndex);
        }

        /* END of REQUIRED FUNCTIONS by ASSIGNMENT #1 */

        const uint8_t addDigitalInput(const uint8_t sensorChannel, const uint8_t value)
        {
            return addField(DATA_TYPES::DIG_IN, sensorChannel, value);
        }

        const uint8_t addDigitalOutput(const uint8_t sensorChannel, const uint8_t value)
        {
            return addField(DATA_TYPES::DIG_OUT, sensorChannel, value);
        }

        const uint8_t addAnalogInput(const uint8_t sensorChannel, const float value)
        {
            return addField(DATA_TYPES::ANL_IN, sensorChannel, value);
        }

        const uint8_t addAnalogOutput(const uint8_t sensorChannel, const float value)
        {
            return addField(DATA_TYPES::ANL_OUT, sensorChannel, value);
        }

        const uint8_t addIllumination(const uint8_t sensorChannel, const uint16_t value)
        {
            return addField(DATA_TYPES::ILLUM_SENS, sensorChannel, value);
        }

        const uint8_t addPresence(const uint8_t sensorChannel, const uint8_t value)
        {
            return addField(DATA_TYPES::PRSNC_SENS, sensorChannel, value);
        }

        const uint8_t addTemperature(const uint8_t sensorChannel, const float value)
        {
            return addField(DATA_TYPES::TEMP_SENS, sensorChannel, value);
        }

        const uint8_t addHumidity(const uint8_t sensorChannel, const float value)
        {
            return addField(DATA_TYPES::HUM_SENS, sensorChannel, value);
        }

        const uint8_t addAccelerometer(const uint8_t sensorChannel, const float x, const float y, const float z)
        {
            return addField(DATA_TYPES::ACCRM_SENS, sensorChannel, x, y, z);
        }

        const uint8_t addBarometer(const uint8_t sensorChannel, const float value)
        {
            return addField(DATA_TYPES::BARO_SENS, sensorChannel, value);
        }

        const uint8_t addGyroscope(const uint8_t sensorChannel, const float x, const float y, const float z)
        {
            return addField(DATA_TYPES::GYRO_SENS, sensorChannel, x, y, z);
        }

        const uint8_t addGPSLocation(uint8_t sensorChannel, const float lat, const float lon, const float alt)
        {
            return addField(DATA_TYPES::GPS_LOC, sensorChannel, lat, lon, alt);
        }

    private:
        uint8_t buffer[MaxSize];
        size_t operationalSize;
        size_t currentIndex;

        const static inline int32_t round_and_cast(const float value)
        {
            if (value > 0)
            {
                return static_cast<int32_t>(value + 0.5f);
            }
            else
            {
                return static_cast<int32_t>(value - 0.5f);
            }
        }

        const static inline int16_t round_and_cast_int16(const float value)
        {
            if (value > 0)
            {
                return static_cast<int16_t>(value + 0.5f);
            }
            else
            {
                return static_cast<int16_t>(value - 0.5f);
            }
        }

        const bool checkCapacity(const size_t dataSize)
        {
            return currentIndex + dataSize <= operationalSize;
        }

        void appendHeader(const DATA_TYPES dataType, const uint8_t sensorChannel)
        {
            buffer[currentIndex++] = static_cast<uint8_t>(dataType);
            buffer[currentIndex++] = sensorChannel;
        }

        template <typename T>
        void appendData(const T data)
        {
            memcpyAVR(&buffer[currentIndex], &data, sizeof(T));
            currentIndex += sizeof(T);
        }

        const uint8_t addFieldImpl(const DATA_TYPES dataType, const uint8_t sensorChannel, const uint8_t value)
        {
            if (!checkCapacity(3)) {
                return 0;
            }
            appendHeader(dataType, sensorChannel);
            buffer[currentIndex++] = value;
            return currentIndex;
        }

        const uint8_t addFieldImpl(const DATA_TYPES dataType, const uint8_t sensorChannel, const uint16_t value)
        {
            if (!checkCapacity(4)) {
                return 0;
            }
            appendHeader(dataType, sensorChannel);
            appendData(value);
            return currentIndex;
        }

        const uint8_t addFieldImpl(const DATA_TYPES dataType, const uint8_t sensorChannel, const float value)
        {
            const uint16_t resolution = FLOATING_DATA_RESOLUTION(dataType);
            int16_t scaledValue = round_and_cast_int16(value * resolution);
            if (!checkCapacity(4)) {
                return 0;
            }
            appendHeader(dataType, sensorChannel);
            appendData(scaledValue);
            return currentIndex;
        }

        const uint8_t addFieldImpl(const DATA_TYPES dataType, const uint8_t sensorChannel, 
            const float first, const float second, const float third)
        {
            const size_t totalBytes = getDataTypeSize(dataType) + 2;
            if (!checkCapacity(totalBytes))
                return 0;

            appendHeader(dataType, sensorChannel);

            if (dataType == DATA_TYPES::GPS_LOC)
            {
                // Special handling for GPS data
                appendData(round_and_cast(first * FLOATING_DATA_RESOLUTION(dataType)));
                appendData(round_and_cast(second * FLOATING_DATA_RESOLUTION(dataType)));
                appendData(round_and_cast(third * (FLOATING_DATA_RESOLUTION(dataType) / 100)));
            }
            else
            {
                // Generic handling for other types requiring three floats
                appendData(round_and_cast_int16(first * FLOATING_DATA_RESOLUTION(dataType)));
                appendData(round_and_cast_int16(second * FLOATING_DATA_RESOLUTION(dataType)));
                appendData(round_and_cast_int16(third * FLOATING_DATA_RESOLUTION(dataType)));
            }
            return currentIndex;
        }

        template <typename... Args>
        const uint8_t addField(const DATA_TYPES dataType, const uint8_t sensorChannel, Args... args)
        {
            return addFieldImpl(dataType, sensorChannel, args...);
        }

        static inline void memcpyAVR(void *dest, const void *src, const size_t n)
        {
            uint8_t *cdest = static_cast<uint8_t*>(dest);
            const uint8_t *csrc = static_cast<const uint8_t*>(src);
            for (size_t i = 0; i < n; i++)
            {
                cdest[i] = csrc[i];
            }
        }
    }; // End of class CayenneLPP.
} // End of Namespace PAYLOAD_ENCODER.
#endif // CAYENNE_LPP_HPP

```

