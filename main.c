//  STM32F103-CMSIS-I2C-LCD-AHT10
//      Version 1.0   07/19/2023    Updated Comments and core files
//
//  Target Microcontroller: STM32F103 (Blue Pill)
//  Mike Shegedin, 05/2023
//
//  Reads in temperature and humidity data from an AHT10 I2C temperature and humidity sensor,
//  and displays this data along with comfort level phrases to an I2C-driven LCD module.
//
//  Target I2C devices:
//    I2C2: AHT10 Temperature and Humidity Module
//    I2C1: 16x2 LCD Driver Module based on the PCF8574.
//
// ================================================================== 
//     ***  HARDWARE SETUP  ***
//
//             I2C LCD Driver Module
//     ======================================
//     1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
//     ======================================
//                16x2 LCD Module
//
//
//     AHT10 Module  Blue Pill  POWER  I2C LCD Driver Module
//     ============  =========  =====  =====================
//                      GND ---- GND
//                       5V ---- 5V
//
//         |  VIN -------------- 3.3V
//    AHT10|  GND -------------- GND
//         |  SCL ----- B6 |I2C1
//         |  SDA ----- B7 |
//
//
//           I2C2|  |   B10 ---------------- SCL
//               |  |   B11 ---------------- SDA
//                  |             5V ------- VCC
//    I2C LCD Module|            GND ------- GND
//                  |
//                  |                        LED Jumper -- [1K] --,
//                  |                                             |
//                  |                        LED Jumper ----------'
//
// ================================================================== 


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "stm32f103xb.h"                  // Primary CMSIS header file

#include "STM32F103-CMSIS-I2C-LCD-lib.c"  // I2C LCD driver library
#include "STM32F103-CMSIS-AHT10-lib.c"    // AHT10 sensor library


//  float
//  heatIndex( float tempC, float humid )
//  Returns the "heat index" given the temperature in Celsius and relative humidity as a
//  whole number, i.e. 67.3% humidity is represented as 67 (not 0.673). Note that the values
//  returned by this function are not likely to be particularly accurate for temperatures
//  below 20 degrees Celsius or humidity values below 40%.
//  Constants and formula for this routine taken from:
//    https://en.wikipedia.org/wiki/Heat_index
float
heatIndex( float t, float r )
{
  float htIdx;  // Holds calculated heat index value
/*
//  These constants are for Fahrenheit calculations:
  const float c1 = -42.379;
  const float c2 =   2.04901523;
  const float c3 =  10.14333127;
  const float c4 =  -0.22475541;
  const float c5 =  -6.83783e-3;
  const float c6 =  -5.481717e-2;
  const float c7 =   1.22874-3;
  const float c8 =   8.5282e-4;
  const float c9 =  -1.99e-6;
*/

//  These following constants are for Celsius calculations:
  const float c1 = -8.78470    ;
  const float c2 =  1.61139    ;
  const float c3 =  2.33855    ;
  const float c4 = -1.46116e-1 ;
  const float c5 = -1.23081e-2 ;
  const float c6 = -1.64248e-2 ;
  const float c7 =  2.21173e-3 ;
  const float c8 =  7.25460e-4 ;
  const float c9 = -3.58200e-6 ;
  
  htIdx = c1 + c2*t + c3*r + c4*t*r + c5*t*t + c6*r*r + c7*t*t*r + c8*t*r*r + c9*t*t*r*r;

  return htIdx;
}  


//  void
//  outFuzzyHeatIndex( int heatIndex )
//  Output a 10-character string to the LCD describing the current heat index in English.
void
outFuzzyHeatIndex( int temp )
{
  if( temp < 1 )
    I2C_LCD_puts("Freezing!!");
  else if( temp < 11 )
    I2C_LCD_puts("Very cold!");
  else if( temp < 18 )
    I2C_LCD_puts("It's cold.");
  else if( temp < 28 )
    I2C_LCD_puts("It's nice!");
  else if( temp < 35 )
    I2C_LCD_puts("Very warm!");
  else
    I2C_LCD_puts("Too hot!!!");
}


// ============================================================================================
// main
// ============================================================================================
int
main()
{
  uint8_t  ahtStatus;               // Status byte returned from the sensor
  char     myString[16];            // Will hold printable strings
  int16_t  temp100, humid100;       // Used in conversion from raw to real data
  float    rTemp, rHumid, heatIdx;  // Used to pass values to/from heat index routine

  I2C_LCD_init( I2C2 );             // Set the LCD interface to I2C1 and initialize it
  I2C_LCD_cmd( LCD_4B_58F_2L );     // Get LCD into 4-bit mode
  I2C_LCD_cmd( LCD_ON_NO_CURSOR );  // LCD ON, Cursor OFF
  I2C_LCD_cmd( LCD_CLEAR );         // Clear the LCD screen
  delay_us( 2e3 );
  I2C_LCD_cmd( LCD_HOME );          // Set the LCD to the home position

  AHT10_init( I2C2 );               // Initialize AHT10 sensor, set sensor to I2C2


  while ( 1 )                           // Repeat this block forever
  {
    //AHT10_readSensorData( gotData );  // Get data from sensor
    ahtStatus = AHT10_getTempHumid100( &temp100, &humid100  );
    
    // Display Heartbeat Character
    I2C_LCD_cmd( LCD_1ST_LINE + 15 );   // Position LCD to the end of the 1st line
    
    if( ahtStatus == 0x19 )             // Display heartbeat
      I2C_LCD_putc( 0xA5 );             // Display center dot to indicate normal operation 
    else
      I2C_LCD_putc( 'E' );              // Display E to indicate error
      
    delay_us( 400e3 );                  // Show heartbeat sign for 400 ms
    I2C_LCD_putc( 0x08 );               // Backspace to clear heartbeat character

    // Separate out humidity and temperature data
    i100toa( temp100, myString );       // Convert x100 value to readable string with one
                                        // decimal place.
    
    I2C_LCD_cmd( LCD_1ST_LINE + 2 );    // Position LCD to display temperature and humidity
    I2C_LCD_puts( myString );           // Display rounded single-decimal-place value
    I2C_LCD_putc( 0xDF );               // Display the degree character
    I2C_LCD_puts( "C  " );

    itoa( humid100, myString, 10);      // humidity % = ( humidV / 2^20 ) * 100
    I2C_LCD_puts( myString );           // which is equiv. to ( humidV / 10486 ).
    I2C_LCD_puts( "%     " );           // Display % character and spaces to ensure the old
                                        // display is cleared.

    I2C_LCD_cmd( LCD_2ND_LINE );        // Display the temperature index value
    I2C_LCD_puts( " Feels like " );
    rTemp   = temp100 / 100;            // Get "real" floating point temperature value
    rHumid  = humid100;                 // get "real" floating point humidity value
    heatIdx = heatIndex ( rTemp, rHumid );  // Get heat index

    // Round floating heatIdx to nearest int
    if( heatIdx < 0 )
      heatIdx -= 0.5;
    else
      heatIdx += 0.5;
    
    itoa( (int)heatIdx, myString, 10 );  // Convert integer part of 
    I2C_LCD_puts( myString );
    I2C_LCD_putc( 0xDF );

    delay_us( 5e6 );                    // Pause approx. 1:0 s between measurements. Excessive
                                        // measurements can lead to self-heating of the sensor.
    I2C_LCD_cmd( LCD_2ND_LINE );
    I2C_LCD_puts( "   " );
    outFuzzyHeatIndex( heatIdx );
    I2C_LCD_puts( "   " );

    delay_us( 5e6 );
  }
  return 1;
}
