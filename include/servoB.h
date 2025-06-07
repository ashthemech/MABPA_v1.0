/* 

 * File:   peripheralTest.h
 * Author: Ashton Coons
 * Brief: Header for test harness for the required MABPA project peripherals on the Teensy 4.1
 * Created on 02.20.2025, 1:53pm
 * Modified on 02.20.2025, 2:04pm
 */

 #ifndef SERVO_B_H // Header guard
 #define SERVO_B_H //
 
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
 /*******************************************************************************
 Function
   screenInit
 Parameters
   none
 Returns
   false if any of the screen functions failed, true if all succeeded
 Description
   initializes the TFT screen for SPI communication, sets the orientation, 
   and sets the initial text color and font.
 Notes

 Author
   Ashton Coons, 5/12/25,
 ****************************************************************************/
  bool servoInit();

  void servoBrake();

  void servoRelease();

 #endif	/* PERIPHERAL_TEST_H */ // End of header guard