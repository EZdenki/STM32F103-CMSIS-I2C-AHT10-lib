# STM32F103-CMSIS-I2C-LCD-AHT10-lib
Library to read the AHT10 I2C temperature and humidity sensor. Includes a sample project that reads the sensor
and displays the temperature, humidity, temperature index, and English phrases. to an I2C-driven 16x2 LCD module.
<br><br>
The I2C LCD Driver Module is hooked up to the I2C1 port (pins B6/B7) and the AHT10 sensor module is
hooked up to I2C2 (pins B10/B11).
<br>
Uses the following simple include libraries:
- STM32F103-pause-lib.c          (Contains us timing routine)
- STM32F103-CMSIS-I2C-lib.c      (Contiains root I2C routines)
- STM32F103-CMSIS-I2C-LCD-lib.c  (Contains I2C LCD Driver Module routines)
- STM32F103-CMSIS-I2C-LCD-AHT10-lib.c  (Contains AHT10 I2C routines to get temperature and humidity.)

See main.c for details.
