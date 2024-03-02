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
/// \author Remko Welling (remko.welling@han.nl)

#include "HAN_IoT_SignalGenerator.h"

#include <math.h> // fabs()

// Base class "waveform" function implementations 
// ==============================================
waveform::waveform(double maximum, double minimum):
  _minimumValue(minimum),
  _maximumValue(maximum),
  _step(0)
{
}

/// \todo check for division by zero???
/// source: https://github.com/openframeworks/openFrameworks/blob/58e47e329d4a2717b5211755a1cb2e65a1a5e1a9/libs/openFrameworks/math/ofMath.cpp
float waveform::floatMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) 
{
  if (fabs(inputMin - inputMax) < FLT_EPSILON)
  {
    return outputMin;
  } 
  else
  {
    float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
  
    if( clamp )
    {
      if(outputMax < outputMin)
      {
        if( outVal < outputMax )outVal = outputMax;
        else if( outVal > outputMin )outVal = outputMin;
      }
      else
      {
        if( outVal > outputMax )outVal = outputMax;
        else if( outVal < outputMin )outVal = outputMin;
      }
    }
    return outVal;
  }
}

unsigned int waveform::getStep(unsigned int actualStep)
{
  unsigned int evaluatedStep = actualStep;
  if(actualStep < (STEP_COUNT_HALF_PERIOD * 2) -1)
  {
      evaluatedStep++;
  }
  else
  {
      evaluatedStep = 0;
  }
  return evaluatedStep;
}

// Derived class "Sine" function implementation
// ============================================
float sine::getSignal()
{
  double sinRad = static_cast<double>(_step)/STEP_COUNT_HALF_PERIOD;
  float sine = static_cast<float>(1.0 + (sin(PIE * sinRad)));
  float amplitude = floatMap(sine, SMALL, BIG, _minimumValue, _maximumValue);
  _step = getStep(_step);
  return amplitude;
}

// Derived class "Cosine" function implementation
// ============================================
float cosine::getSignal()
{
  double sinRad = static_cast<double>(_step)/STEP_COUNT_HALF_PERIOD;
  float sine = static_cast<float>(1.0 + (cos(PIE * sinRad)));
  float amplitude = floatMap(sine, SMALL, BIG, _minimumValue, _maximumValue);
  _step = getStep(_step);
  return amplitude;
}

// Derived class "Triangle" function implementation
// ============================================
float triangle::getSignal()
{
  float triangle = SMALL;

  if(_step <= (STEP_COUNT_HALF_PERIOD / BIG))
  {
      triangle = _step * (BIG / (STEP_COUNT_HALF_PERIOD / 2.0));
  }
  else if ((_step > (STEP_COUNT_HALF_PERIOD / 2.0)) &&
           (_step <= STEP_COUNT_HALF_PERIOD))
  {
      float relativeStep = _step - (STEP_COUNT_HALF_PERIOD / 2.0);
      triangle = BIG - relativeStep * (BIG / (STEP_COUNT_HALF_PERIOD / 2.0));
  }
  else if ((_step > STEP_COUNT_HALF_PERIOD) &&
           (_step <= STEP_COUNT_HALF_PERIOD * 1.5 ))
  {
      float relativeStep = _step - STEP_COUNT_HALF_PERIOD;
      triangle =relativeStep * (BIG / (STEP_COUNT_HALF_PERIOD / 2.0));
  }
  else
  {
      float relativeStep = _step - (STEP_COUNT_HALF_PERIOD * 1.5);
      triangle = BIG - relativeStep * (BIG / (STEP_COUNT_HALF_PERIOD / 2.0));
  }

  float amplitude = floatMap(triangle, SMALL, BIG, _minimumValue, _maximumValue);
  _step = getStep(_step);
  return amplitude;
}

// Derived class "Rectangle" function implementation
// ============================================
float rectangle::getSignal()
{
  float rectangle = SMALL;

  if(_step <= STEP_COUNT_HALF_PERIOD)
  {
      rectangle = BIG;
  }
  else
  {
      rectangle = SMALL;
  }

  float amplitude = floatMap(rectangle, SMALL, BIG, _minimumValue, _maximumValue);
  _step = getStep(_step);
  return amplitude;
}

// Derived class "Sawtooth" function implementation
// ============================================
float sawtooth::getSignal()
{
  float sawtooth = _step * (BIG / (STEP_COUNT_HALF_PERIOD * 2));
  float amplitude = floatMap(sawtooth, SMALL, BIG, _minimumValue, _maximumValue);
  _step = getStep(_step);
  return amplitude;
}
