# STM32F103-CMSIS-I2C-AHT10-lib
### Library to Initialize and Read the AHT10 I2C Temperature and Humidity Sensor
### The STM32F103-CMSIS-I2C-AHT10-lib.c library supports the following routines:
+ ```void AHT10_init( I2C_TypeDef *this I2C )```<br>
  Initializes the specified I2C interface and associates that interface with the AHT10_ routines.
  Then initializes the AHT10 sensor to its default calibrated values.
+ ```void AHT10_readSensorData( uint8_t *data )```<br>
  Reads in the status register and raw temperature and humidity data from the sensor, and places that data into the data array.
+ ```uint8_t AHT10_getTempHumid100( int16_t *temp100, int16_t *humid100 )```<br>
  Calls AHT10_readSensorData and sets temp100 to be the temperature x100 and sets humid100 to be the humidity x100. For example, if the
  temperature and humidity are 23.4 C and 67%, the values returned are 23400 and 67. The return value from the routine is the status byte
  returned from the sensor.<br>
+ See STM32F103-CMSIS-AHT10-lib.c for further details.
### The STMF103-CMSIS-AHT10-lib.c library requires the following libraries to operate:
- STM32F103-CMSIS-I2C-lib
- STM32F103-Pause-lib.c
### Sample Application
- Includes a sample project that reads the sensor and displays the temperature, humidity, temperature index,
  and English phrases. to an I2C-driven 16x2 LCD module.
- The I2C LCD Driver Module is hooked up to the I2C1 port (pins B6/B7) and the AHT10 sensor module is
    hooked up to I2C2 (pins B10/B11).
- To run the sample sample 16x2 LCD project, the following LCD library is also required:<br>
  STM32F103-CMSIS-I2C-LCD-lib.c
- See main.c for further details.
