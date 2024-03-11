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

#ifndef CAYENNE_LPP_HPP
#define CAYENNE_LPP_HPP

#include <cstring> // For memcpy
#include <stdint.h>
#include "CayenneReferences.hpp"

#define PAYLOAD_BUFFER_MAX 0xFF

namespace PAYLOAD_ENCODER
{
    /**
     * @brief Template class for CayenneLPP payload encoder.
     * 
     * @tparam MaxSize Maximum size of the buffer.
     */
    template <size_t MaxSize>
    class CayenneLPP
    {
    public:
        /**
         * @brief Constructor for CayenneLPP.
         * 
         * @param size Size of the buffer.
         */
        explicit CayenneLPP(uint8_t size) : operationalSize(size > MaxSize ? MaxSize : size)
        {
            buffer = new uint8_t[operationalSize]; // Dynamic Buffer Allocation.
            currentIndex = 0;                      // Initialize at construction.
        }

        /**
         * @brief Destructor for CayenneLPP.
         */
        ~CayenneLPP()
        {
            delete[] buffer; // Give the Allocated Memory back, to avoid leaks.
        };

        /* REQUIRED FUNCTIONS by ASSIGNMENT #1 */
        /**
         * @brief Resets the buffer.
         */    
        void reset()
        {
            currentIndex = 0; // Reset currentIndex when buffer is reset
        }

        /**
         * @brief Gets the size of the buffer.
         * 
         * @return size_t Size of the buffer.
         */
        size_t getSize(void) const
        {
            return currentIndex; // Returns the count of used bytes
        }

        /**
         * @brief Returns the buffer.
         * 
         * @return const uint8_t* Pointer to the buffer.
         */
        uint8_t *getBuffer(void)
        {
            return buffer;
        }

        /**
         * @brief Copies the buffer to the destination buffer.
         * 
         * @param destBuffer Destination buffer.
         * @return uint8_t Number of bytes copied.
         */
        uint8_t copy(uint8_t *destBuffer) const
        {
            if (! destBuffer)
            {
                return 0; // Safety check to ensure the destination buffer is valid
            }
            memcpy(destBuffer, buffer, currentIndex);
            return static_cast<uint8_t>(currentIndex);
        }
        
        /* END of REQUIRED FUNCTIONS by ASSIGNMENT #1 */

        // Overload for a single byte value
        const uint8_t addFieldImpl(DATA_TYPES dataType, uint8_t sensorChannel, uint8_t value) {
            auto expectedSize = currentIndex + getDataTypeSize(dataType) + 2;
            if (expectedSize > operationalSize) {
                return 0; // Indicate error
            }

            buffer[currentIndex++] = sensorChannel;
            buffer[currentIndex++] = static_cast<uint8_t>(dataType);
            buffer[currentIndex++] = value;
            return currentIndex; 
        }

        // Overload for a single float value
        const uint8_t addFieldImpl(DATA_TYPES dataType, uint8_t sensorChannel, float value) {
            const uint16_t resolution = FLOATING_DATA_RESOLUTION(dataType);
            int32_t scaledValue = round_and_cast(value * resolution);

            const size_t expectedSize = currentIndex + 2 + sizeof(scaledValue);

            if (expectedSize > operationalSize) {
                return 0;
            }

            buffer[currentIndex++] = static_cast<uint8_t>(dataType);
            buffer[currentIndex++] = sensorChannel;
            
            // Store the scaledValue in the buffer (considering little-endian format; adjust if necessary)
            memcpy(&buffer[currentIndex], &scaledValue, sizeof(scaledValue));
            currentIndex += sizeof(scaledValue);
            return currentIndex; 
        }

        // Overload for GPS coordinates (float, float, float)
        const uint8_t addFieldImpl(DATA_TYPES dataType, uint8_t sensorChannel, float lat, float lon, float alt) {
            const uint16_t resolution = FLOATING_DATA_RESOLUTION(dataType);

            // Scale the float values by their resolution
            int32_t scaledLat = round_and_cast(lat * resolution);
            int32_t scaledLon = round_and_cast(lon * resolution);
            int32_t scaledAlt = round_and_cast(alt * resolution/100); // Dirty quick fix for this value.

            const size_t expectedSize = currentIndex + 2 + sizeof(scaledLat) + sizeof(scaledLon) + sizeof(scaledAlt);
            if (expectedSize > operationalSize) {
                return 0;
            }

            buffer[currentIndex++] = static_cast<uint8_t>(dataType);
            buffer[currentIndex++] = sensorChannel;

            memcpy(&buffer[currentIndex], &scaledLat, sizeof(scaledLat));
            currentIndex += sizeof(scaledLat);
            memcpy(&buffer[currentIndex], &scaledLon, sizeof(scaledLon));
            currentIndex += sizeof(scaledLon);
            memcpy(&buffer[currentIndex], &scaledAlt, sizeof(scaledAlt));
            currentIndex += sizeof(scaledAlt);

            return currentIndex;
        }

        // Variadic template to dispatch to the correct implementation based on argument count and types
        template<typename... Args>
        uint8_t addField(DATA_TYPES dataType, uint8_t sensorChannel, Args... args) {
            return addFieldImpl(dataType, sensorChannel, args...);
        }

    private:
        uint8_t *buffer;
        size_t operationalSize;
        size_t currentIndex;

    static inline int32_t round_and_cast(const float value) {
        if (value > 0) {
            return static_cast<int32_t>(value + 0.5f);
        } else {
            return static_cast<int32_t>(value - 0.5f);
        }
    }
    }; // End of class CayenneLPP.
} // End of Namespace PAYLOAD_ENCODER.
#endif // CAYENNE_LPP_HPP