

# Namespace PAYLOAD\_ENCODER



[**Namespace List**](namespaces.md) **>** [**PAYLOAD\_ENCODER**](namespacePAYLOAD__ENCODER.md)




















## Classes

| Type | Name |
| ---: | :--- |
| class | [**CayenneLPP**](classPAYLOAD__ENCODER_1_1CayenneLPP.md) &lt;MaxSize&gt;<br>_Template class for_ [_**CayenneLPP**_](classPAYLOAD__ENCODER_1_1CayenneLPP.md) _payload encoder._ |


## Public Types

| Type | Name |
| ---: | :--- |
| enum uint8\_t | [**DATA\_TYPES**](#enum-data_types)  <br>_Enum class defining data types for Cayenne LPP._  |
| enum size\_t | [**DATA\_TYPES\_SIZES**](#enum-data_types_sizes)  <br>_Enum class defining sizes of data types for Cayenne LPP._  |
| enum uint8\_t | [**ERROR\_TYPES**](#enum-error_types)  <br>_Enum class defining error types for Cayenne LPP._  |






















## Public Static Functions

| Type | Name |
| ---: | :--- |
|  const int16\_t | [**FLOATING\_DATA\_RESOLUTION**](#function-floating_data_resolution) (DATA\_TYPES dataType) <br>_Function to get the resolution for floating point data types._  |
|  const size\_t | [**getDataTypeSize**](#function-getdatatypesize) (DATA\_TYPES dataType) <br>_Function to create a mapping between data types reference and respective size in bytes._  |


























## Public Types Documentation




### enum DATA\_TYPES 

```C++
enum PAYLOAD_ENCODER::DATA_TYPES {
    DIG_IN = 0,
    DIG_OUT = 1,
    ANL_IN = 2,
    ANL_OUT = 3,
    ILLUM_SENS = 101,
    PRSNC_SENS = 102,
    TEMP_SENS = 103,
    HUM_SENS = 104,
    ACCRM_SENS = 113,
    BARO_SENS = 115,
    GYRO_SENS = 134,
    GPS_LOC = 136
};
```






### enum DATA\_TYPES\_SIZES 

```C++
enum PAYLOAD_ENCODER::DATA_TYPES_SIZES {
    DIG_IN = 1,
    DIG_OUT = 1,
    ANL_IN = 2,
    ANL_OUT = 2,
    ILLUM_SENS = 2,
    PRSNC_SENS = 1,
    TEMP_SENS = 2,
    HUM_SENS = 2,
    ACCRM_SENS = 6,
    BARO_SENS = 2,
    GYRO_SENS = 6,
    GPS_LOC = 12
};
```






### enum ERROR\_TYPES 

```C++
enum PAYLOAD_ENCODER::ERROR_TYPES {
    LPP_ERROR_OVERFLOW = 0,
    LPP_ERROR_UNKOWN_TYPE = 1,
    LPP_ERROR_OK = 2
};
```



## Public Static Functions Documentation




### function FLOATING\_DATA\_RESOLUTION 

_Function to get the resolution for floating point data types._ 
```C++
static inline const int16_t PAYLOAD_ENCODER::FLOATING_DATA_RESOLUTION (
    DATA_TYPES dataType
) 
```





**Parameters:**


* `dataType` The data type. 



**Returns:**

The resolution of the data type. 





        



### function getDataTypeSize 

_Function to create a mapping between data types reference and respective size in bytes._ 
```C++
static inline const size_t PAYLOAD_ENCODER::getDataTypeSize (
    DATA_TYPES dataType
) 
```





**Parameters:**


* `dataType` The data type. 



**Returns:**

The size of the data type in bytes. 





        

------------------------------
The documentation for this class was generated from the following file `include/CayenneLPP.hpp`

