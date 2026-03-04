/* 

 * File:   peripheralTest.h
 * Author: Ashton Coons
 * Brief: Header for test harness for the required MABPA project peripherals on the Teensy 4.1
 * Created on 02.20.2025, 1:53pm
 * Modified on 02.20.2025, 2:04pm
 */

 #ifndef MYOWARE_SENSOR_H // Header guard
 #define MYOWARE_SENSOR_H //
 
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
 
 void muscleSensorInit();

 void readMuscleSensor();

 int getSensorVal();

 bool isSampleReady();

 void clearSampleFlag();

 int muscleValue();

 void collectSamples(int flexVal);

 void resetEMGData();

 int findMinEMG();

 int findMaxEMG();

 #endif	/* PERIPHERAL_TEST_H */ // End of header guard