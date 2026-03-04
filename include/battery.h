/* 

 * File:   battery.h
 * Author: Ashton Coons
 * Brief: Header for the battery source file
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
/*******************************************************************************
 Function
   batteryInit
 Parameters
   none
 Returns
   false if any of the battery functions failed, true if all succeeded
 Description
   initializes the battery by attaching the pins, setting mode to input, and initializes
   the battery readings array to ensure battery percent is shown upon startup
 Notes

 Author
   Ashton Coons, 5/12/25,
 ****************************************************************************/
 bool batteryInit();

 /*******************************************************************************
 Function
   readBatteryVoltage
 Parameters
   none
 Returns
   scaled ADC voltage of battery
 Description
   uses analogRead to read the battery voltage
 Notes

 Author
   Ashton Coons, 5/12/25,
 ****************************************************************************/
 float readBatteryVoltage();

 float getBatteryPercent(float voltage);

 void updateBatteryDisplay(ILI9341_t3 &tft);

 bool updateBatteryReading();

 void updateBatterySamples();

 #endif	/* BATTERY_H */ // End of header guard