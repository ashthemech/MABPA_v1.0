/* 

 * File:   peripheralTest.h
 * Author: Ashton Coons
 * Brief: Header for test harness for the required MABPA project peripherals on the Teensy 4.1
 * Created on 02.20.2025, 1:53pm
 * Modified on 02.20.2025, 2:04pm
 */

 #ifndef BATTERY_H // Header guard
 #define BATTERY_H //

 #include <ILI9341_t3.h>
 
 /*******************************************************************************
  * PUBLIC #INCLUDES                                                            *
  ******************************************************************************/
 
 /*******************************************************************************
  * PUBLIC #DEFINES                                                             *
  ******************************************************************************/

 /*******************************************************************************
  * PUBLIC TYPEDEFS                                                             *
  ******************************************************************************/

 /*******************************************************************************
  * PUBLIC FUNCTION PROTOTYPES                                                  *
  ******************************************************************************/
 
 void batteryInit();

 float readBatteryVoltage();

 float getBatteryPercent(float voltage);

 void updateBatteryDisplay(ILI9341_t3 &tft, int percent);

 #endif	/* BATTERY_H */ // End of header guard