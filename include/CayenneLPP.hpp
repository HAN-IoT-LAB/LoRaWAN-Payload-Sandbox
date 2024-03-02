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

#include <stdint.h>
#include "CayenneReferences.hpp"

#define PAYLOAD_BUFFER_MAX 0xFF

namespace PAYLOAD_ENCODER
{
    template <size_t MaxSize>
    class CayenneLPP
    {
    public:
        explicit CayenneLPP(uint8_t size) : operationalSize(size > MaxSize ? MaxSize : size)
        {
            buffer = new uint8_t[operationalSize]; // Dynamic Buffer Allocation.
            currentIndex = 0;                      // Initialize at construction.
        }

        ~CayenneLPP()
        {
            delete[] buffer; // Give the Allocated Memory back, to avoid leaks.
        };

        /* REQUIRED FUNCTIONS by ASSIGNMENT #1 */
        void reset()
        {
            currentIndex = 0; // Reset currentIndex when buffer is reset
        }

        size_t getSize(void) const
        {
            return currentIndex; // Returns the count of used bytes
        }

        /* @Brief Return buffer by returning the array pointer.
          Const return, to prevent caller of modifications (read-only).
        */
        const uint8_t *getBuffer(void)
        {
            return buffer;
        }

        uint8_t copy(uint8_t *destBuffer) const
        {
            if (!destBuffer)
            {
                return 0; // Safety check to ensure the destination buffer is valid
            }

            // Assuming 'buffer' is the source buffer and 'currentIndex' indicates the number of elements to copy
            for (size_t i = 0; i < currentIndex; ++i)
            {
                destBuffer[i] = buffer[i]; // Manually copy each element
            }

            return static_cast<uint8_t>(currentIndex); // Return the number of elements copied
        }

        /* END of REQUIRED FUNCTIONS by ASSIGNMENT #1 */

        /* Additional Functionality */
        // 1) Add a Bit/Byte;
        void addWord_byte(const uint8_t byte);

        // 2) 16-bit word;
        void addWord_16b(const uint16_t word_16b);

        // 3) 32-bit word;
        void addWord_32b(const uint16_t word_16b);

        // 4) float word;
        void addFloat(const float word_decimal);
        /* END of Additional Functionality */

        // To be Reviewed:
        const uint8_t addDigitalInput(uint8_t channel, uint8_t value)
        {
            auto expectedSize = currentIndex + getDataTypeSize(DATA_TYPES::DIG_IN);
            if (expectedSize > operationalSize)
            {
                // Handle error
                return 0;
            }
            buffer[currentIndex++] = channel;
            buffer[currentIndex++] = static_cast<uint8_t>(DATA_TYPES::DIG_IN);
            buffer[currentIndex++] = value;
            return currentIndex; // Success
        }

        const uint8_t addDigitalOutput(uint8_t channel, uint8_t value)
        {
            auto expectedSize = currentIndex + getDataTypeSize(DATA_TYPES::DIG_OUT);
            if (expectedSize > operationalSize)
            {
                return 0;
            }
            buffer[currentIndex++] = channel;
            buffer[currentIndex++] = static_cast<uint8_t>(DATA_TYPES::DIG_OUT);
            buffer[currentIndex++] = value;

            return currentIndex;
        }

        const uint8_t addAnalogInput(uint8_t channel, float value)
        {
            auto expectedSize = currentIndex + getDataTypeSize(DATA_TYPES::ANL_IN);
            if (expectedSize > operationalSize)
            {
                return 0;
            }

            const int16_t val = value * 100;
            buffer[currentIndex++] = channel;
            buffer[currentIndex++] = static_cast<uint8_t>(DATA_TYPES::ANL_IN);
            buffer[currentIndex++] = val >> 8;
            buffer[currentIndex++] = val;
            return currentIndex;
        }

        uint8_t addAnalogOutput(uint8_t channel, float value)
        {
            auto expectedSize = currentIndex + getDataTypeSize(DATA_TYPES::ANL_OUT);
            if (expectedSize > operationalSize)
            {
                return 0;
            }
            int16_t val = value * 100;
            buffer[currentIndex++] = channel;
            buffer[currentIndex++] = static_cast<uint8_t>(DATA_TYPES::ANL_OUT);
            buffer[currentIndex++] = val >> 8;
            buffer[currentIndex++] = val;

            return currentIndex;
        }

    private:
        uint8_t *buffer;
        size_t operationalSize;
        static inline size_t currentIndex;
    }; // End of class CayenneLPP.
} // End of Namespace PAYLOAD_ENCODER.
#endif // CAYENNE_LPP_HPP