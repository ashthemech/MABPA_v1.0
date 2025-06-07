/* 

 * File:   peripheralTest.h
 * Author: Ashton Coons
 * Brief: Header for test harness for the required MABPA project peripherals on the Teensy 4.1
 * Created on 02.20.2025, 1:53pm
 * Modified on 02.20.2025, 2:04pm
 */

 #ifndef TFT_SCREEN_H // Header guard
 #define TFT_SCREEN_H //
 
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
  bool screenInit();

  void testFont();

/*******************************************************************************
 Function
   drawLogo
 Parameters
   none
 Returns
   false if any of the screen functions failed, true if all succeeded
 Description
   startup graphic for the team name and logo to appear on the screen while 
   the hardware component initalize.
 Notes

 Author
   Ashton Coons, 5/12/25,
 ****************************************************************************/
  void drawLogo();

/*******************************************************************************
 Function
   batteryFail
 Parameters
   none
 Returns
   none
 Description
   prints user warning message if the user tries to use a dead battery 
   of an overvoltage battery for the system, and directs user to
   replace the battery and only use a 2s 7.4V lipo
 Notes
 warning message: "CRITICAL BATTERY ERROR", "PLEASE REPLACE BATTERY", 
                  "USE A 2S 7.4V LIPO ONLY"

 Author
   Ashton Coons, 5/12/25,
 ****************************************************************************/
  void batteryFail();

  void servoFail();

  void touchScreenTest();

  void musclePlacementPrompt();

  bool userReady();

  void muscleFlexPrompt();

  void startFlexTimer();

  bool updateFlexTimer();

  void EMGDataError();

  void EMGDataValid();

  void batteryUpdate();

  void brakeEngaged();

  void brakeReleased();

  void runScreenInit();

  void printMuscle(int muscle);

 #endif	/* PERIPHERAL_TEST_H */ // End of header guard