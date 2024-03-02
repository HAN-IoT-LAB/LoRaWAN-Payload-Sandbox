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

#ifndef CAYENNE_REFERENCES_HPP
#define CAYENNE_REFERENCES_HPP

#include <stdint.h>

namespace PAYLOAD_ENCODER
{
    enum class DATA_TYPES : uint8_t
    {
        DIG_IN = 0,       /* DIGITAL INPUT */
        DIG_OUT = 1,      /* DITIAL OUTPUT */
        ANL_IN = 2,       /* ANALOG INPUT */
        ANL_OUT = 3,      /* ANALOG OUTPUT */
        ILLUM_SENS = 101, /* ILLUMINATION SENSOR */
        PRSNC_SENS = 102, /* PRESCENCE SENSOR */
        TEMP_SENS = 103,  /* TEMPERATURE SENSOR */
        HUM_SENS = 104,   /* HUMIDITY SENSOR */
        ACCRM_SENS = 113, /* ACCELEROMETER */
        BARO_SENS = 115,  /* BAROMETER */
        GYRO_SENS = 134,  /* GYROMETER */
        GPS_LOC = 136     /* GPS LOCATION METER */
    };

    enum class DATA_TYPES_SIZES : size_t
    {
        DIG_IN = 1,     /* 1 bit resolution */
        DIG_OUT = 1,    /* 1 bit resolution */
        ANL_IN = 2,     /* 0.01 Signed */
        ANL_OUT = 2,    /* 0.01 Signed */
        ILLUM_SENS = 2, /* 1 Lux Unsigned MSB */
        PRSNC_SENS = 1, /* 1 bit resolution */
        TEMP_SENS = 2,  /* 0.1 °C Signed MSB */
        HUM_SENS = 1,   /* 0.5 % Unsigned */
        ACCRM_SENS = 6, /* 0.001 G Signed MSB per axis */
        BARO_SENS = 2,  /* 0.1 hPa Unsigned MSB */
        GYRO_SENS = 6,  /* 0.01 °/s Signed MSB per axis */
        GPS_LOC = 9     /* Latitude  : 0.0001° Signed MSB
                         * Longitude : 0.0001° Signed MSB
                         * Altitude  : 0.01 meter Signed MSB */
    };

    // Function to create a mapping between data types reference and respective size in bytes;
    static const size_t getDataTypeSize(DATA_TYPES dataType)
    {
        switch (dataType)
        {
        case DATA_TYPES::DIG_IN:
            return static_cast<size_t>(DATA_TYPES_SIZES::DIG_IN);
        case DATA_TYPES::DIG_OUT:
            return static_cast<size_t>(DATA_TYPES_SIZES::DIG_OUT);
        case DATA_TYPES::ANL_IN:
            return static_cast<size_t>(DATA_TYPES_SIZES::ANL_IN);
        case DATA_TYPES::ANL_OUT:
            return static_cast<size_t>(DATA_TYPES_SIZES::ANL_OUT);
        case DATA_TYPES::ILLUM_SENS:
            return static_cast<size_t>(DATA_TYPES_SIZES::ILLUM_SENS);
        case DATA_TYPES::PRSNC_SENS:
            return static_cast<size_t>(DATA_TYPES_SIZES::PRSNC_SENS);
        case DATA_TYPES::TEMP_SENS:
            return static_cast<size_t>(DATA_TYPES_SIZES::TEMP_SENS);
        case DATA_TYPES::HUM_SENS:
            return static_cast<size_t>(DATA_TYPES_SIZES::HUM_SENS);
        case DATA_TYPES::ACCRM_SENS:
            return static_cast<size_t>(DATA_TYPES_SIZES::ACCRM_SENS);
        case DATA_TYPES::BARO_SENS:
            return static_cast<size_t>(DATA_TYPES_SIZES::BARO_SENS);
        case DATA_TYPES::GYRO_SENS:
            return static_cast<size_t>(DATA_TYPES_SIZES::GYRO_SENS);
        case DATA_TYPES::GPS_LOC:
            return static_cast<size_t>(DATA_TYPES_SIZES::GPS_LOC);
        default:
            return 0; // Or handle unknown data type appropriately
        }
        return 0;
    }
} // End of PAYLOAD_ENCODER Namespace.

#endif // End of CAYENNE_REFERENCES_HPP