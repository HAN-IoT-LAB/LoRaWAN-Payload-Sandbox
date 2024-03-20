

# Class PAYLOAD\_ENCODER::CayenneLPP

**template &lt;size\_t MaxSize&gt;**



[**ClassList**](annotated.md) **>** [**PAYLOAD\_ENCODER**](namespacePAYLOAD__ENCODER.md) **>** [**CayenneLPP**](classPAYLOAD__ENCODER_1_1CayenneLPP.md)



_Template class for_ [_**CayenneLPP**_](classPAYLOAD__ENCODER_1_1CayenneLPP.md) _payload encoder._[More...](#detailed-description)

* `#include <CayenneLPP.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CayenneLPP**](#function-cayennelpp-12) (const uint8\_t size) <br>_Constructor for_ [_**CayenneLPP**_](classPAYLOAD__ENCODER_1_1CayenneLPP.md) _._ |
|   | [**CayenneLPP**](#function-cayennelpp-22) (const [**CayenneLPP**](classPAYLOAD__ENCODER_1_1CayenneLPP.md) & other) <br>_Copy constructor for the_ [_**CayenneLPP**_](classPAYLOAD__ENCODER_1_1CayenneLPP.md) _class._ |
|  const uint8\_t | [**addAccelerometer**](#function-addaccelerometer) (const uint8\_t sensorChannel, const float x, const float y, const float z) <br>_Adds an accelerometer field to the payload._  |
|  const uint8\_t | [**addAnalogInput**](#function-addanaloginput) (const uint8\_t sensorChannel, const float value) <br>_Adds an analog input field to the payload._  |
|  const uint8\_t | [**addAnalogOutput**](#function-addanalogoutput) (const uint8\_t sensorChannel, const float value) <br>_Adds an analog output field to the payload._  |
|  const uint8\_t | [**addBarometer**](#function-addbarometer) (const uint8\_t sensorChannel, const float value) <br>_Adds a barometric pressure field to the payload._  |
|  const uint8\_t | [**addDigitalInput**](#function-adddigitalinput) (const uint8\_t sensorChannel, const uint8\_t value) <br>_Adds a digital input field to the payload._  |
|  const uint8\_t | [**addDigitalOutput**](#function-adddigitaloutput) (const uint8\_t sensorChannel, const uint8\_t value) <br>_Adds a digital output field to the payload._  |
|  const uint8\_t | [**addGPSLocation**](#function-addgpslocation) (uint8\_t sensorChannel, const float lat, const float lon, const float alt) <br>_Adds a GPS location field to the payload._  |
|  const uint8\_t | [**addGyroscope**](#function-addgyroscope) (const uint8\_t sensorChannel, const float x, const float y, const float z) <br>_Adds a gyroscope field to the payload._  |
|  const uint8\_t | [**addHumidity**](#function-addhumidity) (const uint8\_t sensorChannel, const float value) <br>_Adds a humidity field to the payload._  |
|  const uint8\_t | [**addIllumination**](#function-addillumination) (const uint8\_t sensorChannel, const uint16\_t value) <br>_Adds an illumination field to the payload._  |
|  const uint8\_t | [**addPresence**](#function-addpresence) (const uint8\_t sensorChannel, const uint8\_t value) <br>_Adds a presence sensor field to the payload._  |
|  const uint8\_t | [**addTemperature**](#function-addtemperature) (const uint8\_t sensorChannel, const float value) <br>_Adds a temperature field to the payload._  |
|  const uint8\_t | [**copy**](#function-copy) (uint8\_t \* destBuffer) const<br>_Copies the buffer to the destination buffer._  |
|  const uint8\_t \* | [**getBuffer**](#function-getbuffer) (void) const<br>_Returns the buffer._  |
|  size\_t | [**getSize**](#function-getsize) (void) const<br>_Gets the size of the buffer._  |
|  [**CayenneLPP**](classPAYLOAD__ENCODER_1_1CayenneLPP.md) & | [**operator=**](#function-operator) (const [**CayenneLPP**](classPAYLOAD__ENCODER_1_1CayenneLPP.md) & other) <br>_Copy assignment operator for the_ [_**CayenneLPP**_](classPAYLOAD__ENCODER_1_1CayenneLPP.md) _class._ |
|  void | [**reset**](#function-reset) () <br>_Resets the buffer._  |
|   | [**~CayenneLPP**](#function-cayennelpp) () <br>_Destructor for_ [_**CayenneLPP**_](classPAYLOAD__ENCODER_1_1CayenneLPP.md) _._ |




























# Detailed Description




**Template parameters:**


* `MaxSize` Maximum size of the buffer. 




    
## Public Functions Documentation




### function CayenneLPP [1/2]

_Constructor for_ [_**CayenneLPP**_](classPAYLOAD__ENCODER_1_1CayenneLPP.md) _._
```C++
inline explicit PAYLOAD_ENCODER::CayenneLPP::CayenneLPP (
    const uint8_t size
) 
```





**Parameters:**


* `size` Size of the buffer. 




        



### function CayenneLPP [2/2]

_Copy constructor for the_ [_**CayenneLPP**_](classPAYLOAD__ENCODER_1_1CayenneLPP.md) _class._
```C++
inline PAYLOAD_ENCODER::CayenneLPP::CayenneLPP (
    const CayenneLPP & other
) 
```



Initializes a new instance of the [**CayenneLPP**](classPAYLOAD__ENCODER_1_1CayenneLPP.md) class by deep copying the contents from another instance. This includes copying the operational size, current index, and the contents of the data buffer up to the current index. The purpose of this constructor is to create a new object with the same state as the object passed as a parameter.




**Parameters:**


* `other` The [**CayenneLPP**](classPAYLOAD__ENCODER_1_1CayenneLPP.md) instance from which to copy. 




        



### function addAccelerometer 

_Adds an accelerometer field to the payload._ 
```C++
inline const uint8_t PAYLOAD_ENCODER::CayenneLPP::addAccelerometer (
    const uint8_t sensorChannel,
    const float x,
    const float y,
    const float z
) 
```



This method appends accelerometer data to the payload, including x, y, and z axis measurements. The accelerometer data is identified by a unique sensor channel, with each axis value representing acceleration in Gs. This method allows for the capture of three-dimensional motion or orientation data.




**Parameters:**


* `sensorChannel` The channel number of the accelerometer sensor. 
* `x` The acceleration value along the x-axis in Gs. 
* `y` The acceleration value along the y-axis in Gs. 
* `z` The acceleration value along the z-axis in Gs. 



**Returns:**

uint8\_t Returns the new size of the payload after adding the accelerometer data. Returns 0 if there was an error or if the payload could not be appended. 





        



### function addAnalogInput 

_Adds an analog input field to the payload._ 
```C++
inline const uint8_t PAYLOAD_ENCODER::CayenneLPP::addAnalogInput (
    const uint8_t sensorChannel,
    const float value
) 
```





**Parameters:**


* `sensorChannel` The channel number of the analog input sensor. 
* `value` The analog value to be added. This value is scaled appropriately before adding to the payload. Precision of 0.01 Signed. 



**Returns:**

uint8\_t Returns the new size of the payload after adding the analog input. 





        



### function addAnalogOutput 

_Adds an analog output field to the payload._ 
```C++
inline const uint8_t PAYLOAD_ENCODER::CayenneLPP::addAnalogOutput (
    const uint8_t sensorChannel,
    const float value
) 
```





**Parameters:**


* `sensorChannel` The channel number of the analog output sensor. 
* `value` The analog value to be added. This value is scaled appropriately before adding to the payload. Precision of 0.01 Signed. 



**Returns:**

uint8\_t Returns the new size of the payload after adding the analog output. 





        



### function addBarometer 

_Adds a barometric pressure field to the payload._ 
```C++
inline const uint8_t PAYLOAD_ENCODER::CayenneLPP::addBarometer (
    const uint8_t sensorChannel,
    const float value
) 
```



This method appends a barometric pressure value to the payload. The barometric pressure is identified by a unique sensor channel, with the value representing atmospheric pressure in hPa (hectopascals).




**Parameters:**


* `sensorChannel` The channel number of the barometer sensor. 
* `value` The barometric pressure in hPa to be added. This value is scaled appropriately before adding to the payload. 



**Returns:**

uint8\_t Returns the new size of the payload after adding the barometric pressure value. Returns 0 if there was an error or if the payload could not be appended. 





        



### function addDigitalInput 

_Adds a digital input field to the payload._ 
```C++
inline const uint8_t PAYLOAD_ENCODER::CayenneLPP::addDigitalInput (
    const uint8_t sensorChannel,
    const uint8_t value
) 
```





**Parameters:**


* `sensorChannel` The channel number of the digital input sensor. 
* `value` The value of the digital input byte or bool (0 or 1). 



**Returns:**

uint8\_t Returns the new size of the payload after adding the digital input. 





        



### function addDigitalOutput 

_Adds a digital output field to the payload._ 
```C++
inline const uint8_t PAYLOAD_ENCODER::CayenneLPP::addDigitalOutput (
    const uint8_t sensorChannel,
    const uint8_t value
) 
```





**Parameters:**


* `sensorChannel` The channel number of the digital output sensor. 
* `value` The value of the digital output byte or bool (0 or 1). 



**Returns:**

uint8\_t Returns the new size of the payload after adding the digital output. 





        



### function addGPSLocation 

_Adds a GPS location field to the payload._ 
```C++
inline const uint8_t PAYLOAD_ENCODER::CayenneLPP::addGPSLocation (
    uint8_t sensorChannel,
    const float lat,
    const float lon,
    const float alt
) 
```



This method appends GPS location data to the payload, including latitude, longitude, and altitude. The GPS location data is identified by a unique sensor channel, with the latitude and longitude values in degrees and the altitude in meters.




**Parameters:**


* `sensorChannel` The channel number of the GPS sensor. 
* `lat` The latitude in degrees. 
* `lon` The longitude in degrees. 
* `alt` The altitude in meters above sea level. 



**Returns:**

uint8\_t Returns the new size of the payload after adding the GPS location data. Returns 0 if there was an error or if the payload could not be appended. 





        



### function addGyroscope 

_Adds a gyroscope field to the payload._ 
```C++
inline const uint8_t PAYLOAD_ENCODER::CayenneLPP::addGyroscope (
    const uint8_t sensorChannel,
    const float x,
    const float y,
    const float z
) 
```



This method appends gyroscope data to the payload, including angular velocity around the x, y, and z axes. The gyroscope data is identified by a unique sensor channel, with each axis value representing angular velocity in degrees per second.




**Parameters:**


* `sensorChannel` The channel number of the gyroscope sensor. 
* `x` The angular velocity around the x-axis in degrees per second. 
* `y` The angular velocity around the y-axis in degrees per second. 
* `z` The angular velocity around the z-axis in degrees per second. 



**Returns:**

uint8\_t Returns the new size of the payload after adding the gyroscope data. Returns 0 if there was an error or if the payload could not be appended. 





        



### function addHumidity 

_Adds a humidity field to the payload._ 
```C++
inline const uint8_t PAYLOAD_ENCODER::CayenneLPP::addHumidity (
    const uint8_t sensorChannel,
    const float value
) 
```



This method appends a humidity value to the payload, represented as a percentage of relative humidity. The humidity sensor is identified by a unique sensor channel, and the value must be between 0 and 100, representing 0% to 100% relative humidity.




**Parameters:**


* `sensorChannel` The channel number of the humidity sensor. 
* `value` The relative humidity value to be added, specified as a percentage from 0 to 100. This value is scaled appropriately before adding to the payload. 



**Returns:**

uint8\_t Returns the new size of the payload after adding the humidity value. Returns 0 if there was an error or if the payload could not be appended. 





        



### function addIllumination 

_Adds an illumination field to the payload._ 
```C++
inline const uint8_t PAYLOAD_ENCODER::CayenneLPP::addIllumination (
    const uint8_t sensorChannel,
    const uint16_t value
) 
```



This method appends an illumination value (two-byte value) to the payload. The illumination is identified by a unique sensor channel and its value, representing light intensity in lux.




**Parameters:**


* `sensorChannel` The channel number of the illumination sensor. 
* `value` The illumination value in lux to be added. This value is encoded directly into the payload. 



**Returns:**

uint8\_t Returns the new size of the payload after adding the illumination value. 





        



### function addPresence 

_Adds a presence sensor field to the payload._ 
```C++
inline const uint8_t PAYLOAD_ENCODER::CayenneLPP::addPresence (
    const uint8_t sensorChannel,
    const uint8_t value
) 
```



This method appends a presence sensor value (single byte value) to the payload. The presence sensor is identified by a unique sensor channel and its value (0 or 1), indicating absence or presence.




**Parameters:**


* `sensorChannel` The channel number of the presence sensor. 
* `value` The value of the presence sensor (0 for absence, 1 for presence). 



**Returns:**

uint8\_t Returns the new size of the payload after adding the presence sensor value. 





        



### function addTemperature 

_Adds a temperature field to the payload._ 
```C++
inline const uint8_t PAYLOAD_ENCODER::CayenneLPP::addTemperature (
    const uint8_t sensorChannel,
    const float value
) 
```



This method appends a temperature value (two-byte value) to the payload. The temperature is identified by a unique sensor channel and its value, representing the temperature in degrees Celsius.




**Parameters:**


* `sensorChannel` The channel number of the temperature sensor. 
* `value` The temperature value in degrees Celsius to be added. This value is scaled appropriately before adding to the payload. 



**Returns:**

uint8\_t Returns the new size of the payload after adding the temperature value. 





        



### function copy 

_Copies the buffer to the destination buffer._ 
```C++
inline const uint8_t PAYLOAD_ENCODER::CayenneLPP::copy (
    uint8_t * destBuffer
) const
```





**Parameters:**


* `destBuffer` Destination buffer. 



**Returns:**

uint8\_t Number of bytes copied. 





        



### function getBuffer 

_Returns the buffer._ 
```C++
inline const uint8_t * PAYLOAD_ENCODER::CayenneLPP::getBuffer (
    void
) const
```





**Returns:**

const uint8\_t\* Pointer to the buffer. 





        



### function getSize 

_Gets the size of the buffer._ 
```C++
inline size_t PAYLOAD_ENCODER::CayenneLPP::getSize (
    void
) const
```





**Returns:**

size\_t Size of the buffer. 





        



### function operator= 

_Copy assignment operator for the_ [_**CayenneLPP**_](classPAYLOAD__ENCODER_1_1CayenneLPP.md) _class._
```C++
inline CayenneLPP & PAYLOAD_ENCODER::CayenneLPP::operator= (
    const CayenneLPP & other
) 
```



Replaces the contents of this instance with a copy of the contents of another instance.




**Parameters:**


* `other` The [**CayenneLPP**](classPAYLOAD__ENCODER_1_1CayenneLPP.md) instance to assign from. 



**Returns:**

[**CayenneLPP**](classPAYLOAD__ENCODER_1_1CayenneLPP.md)& A reference to this instance after copying. 





        



### function reset 

```C++
inline void PAYLOAD_ENCODER::CayenneLPP::reset () 
```






### function ~CayenneLPP 

```C++
inline PAYLOAD_ENCODER::CayenneLPP::~CayenneLPP () 
```




------------------------------
The documentation for this class was generated from the following file `include/CayenneLPP.hpp`

