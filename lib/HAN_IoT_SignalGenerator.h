/*--------------------------------------------------------------------
  This file is part of the HAN IoT Signal generator.

  This code is free software:
  you can redistribute it and/or modify it under the terms of a Creative
  Commons Attribution-NonCommercial 4.0 International License
  (http://creativecommons.org/licenses/by-nc/4.0/) by
  Remko Welling (https://ese.han.nl/~rwelling/) E-mail: remko.welling@han.nl

  The program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  --------------------------------------------------------------------*/

/// \file HAN_IoT_SignalGenerator.h
/// \brief class to generate various signals
/// \date 10-1-2018
/// \author Remko Welling (remko.welling@han.nl)
/// \version 2.1

/// Version|Date        |Note
/// -------|------------|----
/// 2.1    | 10- 1-2019 | Various changes. Sanity check and documentation update
/// 2.0    | 15-12-2018 | Initial release
/// 1.0    | 15-12-2018 | Initial version

/*! 
 * \todo make sawtooth selectable ramp-up and -down
 * \todo make interval not step but time dependent
 * \todo replace double in base class constructor for float or make double usage consistent
 */

#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include <stdint.h>

const float   PIE         = 3.14159265359;    ///< PI constant
const float   FLT_EPSILON = 0x0.000002p0;     ///< used for comparing two floats
const float   SMALL       = 0.0;              ///< minimum internal signal value
const float   BIG         = 2.0;              ///< maximum internal signal value

const uint8_t STEP_COUNT_HALF_PERIOD = 20;    ///< Number of steps in a period

/// \brief Base class waveform
class waveform
{
public:
  waveform(double maximum, double minimum);
  
  virtual ~waveform(){}
  
  /// \brief returns signal of waveform
  /// This function is pure virtual and is implemented in the derived classes
  virtual float getSignal() = 0;

private:

protected: 
  float _minimumValue;    ///< Minimum value that the waveform will deliver
  float _maximumValue;    ///< Maximum value that the waveform will deliver
  unsigned int _step;     ///< keeps actual step of the sine waveform in a period

  /// \brief get next stap for future calls
  /// \param actualStep at the time the function getSignal() was called.
  /// \return incremented step.
  unsigned int getStep(unsigned int actualStep);

  /// \brief scale and shift a value within a range to a new range
  /// \param value to be mapped
  /// \param inputMin minimum value of the input rangt 
  /// \param inputMax maximum value of the input range
  /// \param outputMin minimum value of the output range
  /// \param outputMax maximum value of the output range
  /// \param clamp; true=yes, false=no.
  /// \return mapped value transposed input value to the output range 
  float floatMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false); 
};

class sine:public waveform
{
public:
//  sine(double maximum, double minimum): waveform(maximum, minimum){}
  using waveform::waveform;
  
  /// \brief returns amplititde at step of sine
  /// This function is the implementation of pure virtual from the base class
  float getSignal() override;
};

class cosine:waveform
{
public:
  using waveform::waveform;
  
  /// \brief returns amplititde at step of cosine
  /// This function is the implementation of pure virtual from the base class
  float getSignal();
};

class triangle:waveform
{
public:
  using waveform::waveform;
  
  /// \brief returns amplititde at step of the triangular waveform
  /// This function is the implementation of pure virtual from the base class
  float getSignal();
};

class rectangle:waveform
{
public:
  using waveform::waveform;
    
  /// \brief returns amplititde at step of the rectangular waveform
  /// This function is the implementation of pure virtual from the base class
  float getSignal();
};

class sawtooth:waveform
{
public:
  using waveform::waveform;
  
  /// \brief returns amplititde at step of the swtooth waveform
  /// This function is the implementation of pure virtual from the base class
  float getSignal();
};


#endif // SIGNALGENERATOR_H
