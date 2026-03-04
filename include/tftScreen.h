/* 

 * File:   tftScreen.h
 * Author: Ashton Coons
 * Brief: header for the tft screen functions used in teh MABPA codebase
 * Created on 02.20.2025, 1:53pm
 * Modified on 05.25.2025, 6:04pm
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

/*******************************************************************************
 Function
   drawLogo
 Parameters
   none
 Returns
   none
 Description
   startup graphic for the team name and logo to appear on the screen while 
   the hardware components initalize
 Notes

 Author
   Ashton Coons, 5/12/25,
 ****************************************************************************/
  void drawLogo();

/*******************************************************************************
 Function
   batteryFailPrompt
 Parameters
   none
 Returns
   none
 Description
   prints user warning message if the user tries to use a dead battery 
   of an overvoltage battery for the system, and directs user to
   replace the battery and only use a 2s 7.4V lipo. If the problem persists
   after the user has done the outlined troubleshooting, it tells the user to
   not use the system.
 Notes
 warning message: "CRITICAL BATTERY ERROR", "PLEASE REPLACE BATTERY", 
                  "USE A 2S 7.4V LIPO ONLY", "IF PROBLEM PERSISTS, DO NOT USE SYSTEM"

 Author
   Ashton Coons, 5/12/25,
 ****************************************************************************/
  void batteryFailPrompt();

  /*******************************************************************************
 Function
   servoFailPrompt
 Parameters
   none
 Returns
   none
 Description
   prints user warning message if the system detects that the servo is disconnected
   from the brake or not fully pulling the brake lever. Prompts user to ensure that
   the mechanical linkage from the servo to the brake is connected, and if the
   problem persists then do not use the system.
 Notes
 warning message: "CRITICAL BRAKE ERROR", "ENSURE ATTACHMENT IS CONNECTED TO THE BRAKE LEVER", 
                  "IF PROBLEM PERSISTS, DO NOT USE SYSTEM"

 Author
   Ashton Coons, 5/15/25,
 ****************************************************************************/
  void servoFailPrompt();

/*******************************************************************************
 Function
   musclePlacementPrompt
 Parameters
   none
 Returns
   none
 Description
   prints a user prompt to the screen that directs user to place the muscle sensor on 
   their bicep and secure it tightly, and to touch the screen when the user is ready
   to test the palcement of the sensor. Then it disables the tft print function and enables
   the tft touch function. 
 Notes
 message: "Place the muscle sensor on your bicep and secure the armband tightly.", 
          "Press the screen when ready to test."

 Author
   Ashton Coons, 5/15/25,
 ****************************************************************************/
  void musclePlacementPrompt();

/*******************************************************************************
 Function
   userReady
 Parameters
   none
 Returns
   true/false
 Description
   detects a touch from the user for feedback when given a prompt. while a touch is
   not detected, returns false. when a touch is detected, returns true.
 Notes

 Author
   Ashton Coons, 5/15/25,
 ****************************************************************************/
  bool userReady();

/*******************************************************************************
 Function
   muscleFlexPrompt
 Parameters
   none
 Returns
   none
 Description
   prints a user prompt to the screen that directs user to flex and unflex their 
   bicep at a good pace and intensity for 15 seconds to collect EMG data and ensure
   the placement of the muscle sensor is valid. Also prints the user prompt to
   touch the screen when ready to start.
 Notes
 message: "Flex and unflex your bicep at a cofortable pace and intensity for 15 seconds.", 
          "Press the screen when ready to start the timer."

 Author
   Ashton Coons, 5/15/25,
 ****************************************************************************/
  void muscleFlexPrompt();

/*******************************************************************************
 Function
   startFlexTimer
 Parameters
   none
 Returns
   none
 Description
   sets the timer active flag to true and records the initial start time for the 
   flex timer countdown.
 Notes

 Author
   Ashton Coons, 5/15/25,
 ****************************************************************************/
  void startFlexTimer();

/*******************************************************************************
 Function
   updateFlexTimer
 Parameters
   none
 Returns
   true/false
 Description
   updates the flex time onscreen by checking if a millis variable has elapsed
   over one second. returns true if the timer is still running, false if the timer has stopped.
 Notes

 Author
   Ashton Coons, 5/18/25,
 ****************************************************************************/
  bool updateFlexTimer();

/*******************************************************************************
 Function
   EMGDataError
 Parameters
   none
 Returns
   none
 Description
   if the EMG data collected during initialization is invalid (<200 spread), then
   this prompts the user to reposition the sensor for a better placement. 
 Notes
message: "EMG PLACEMENT ERROR", "Reposition the sensor." 
          "Press the screen when ready to try again."
 Author
   Ashton Coons, 5/18/25,
 ****************************************************************************/
  void EMGDataError();

/*******************************************************************************
 Function
   EMGDataValid
 Parameters
   none
 Returns
   none
 Description
   if the EMG data collected during initialization is valid(>200 spread), this informs
   the user that collection was sucessful and promps them to start the ride. 
 Notes
message: "EMG Collection Done", 
          "Press the screen when ready to ride!"
 Author
   Ashton Coons, 5/18/25,
 ****************************************************************************/
  void EMGDataValid();
  
/*******************************************************************************
 Function
   batteryUpdate
 Parameters
   none
 Returns
   none
 Description
   calls updateBatteryDisplay so that this function is accessible to other 
   files that do not have the tft object defined.
 Notes

 Author
   Ashton Coons, 5/18/25,
 ****************************************************************************/
  void batteryUpdate();

/*******************************************************************************
 Function
   brakeEngaged
 Parameters
   none
 Returns
   none
 Description
   prints the brake engaged prompt to the screen upon detection of a muscle flex
 Notes
   message: "ENGAGED"
 Author
   Ashton Coons, 5/20/25,
 ****************************************************************************/
  void brakeEngaged();

/*******************************************************************************
 Function
   brakeReleased
 Parameters
   none
 Returns
   none
 Description
   prints the brake released prompt to the screen upon detection of a muscle relax
 Notes
   message: "RELEASED"
 Author
   Ashton Coons, 5/20/25,
 ****************************************************************************/
  void brakeReleased();

/*******************************************************************************
 Function
   runScreenInit
 Parameters
   none
 Returns
   none
 Description
   clears the screen and sets the font size for runtime after initialization
 Notes

 Author
   Ashton Coons, 5/20/25,
 ****************************************************************************/
  void runScreenInit();

/*******************************************************************************
 Function
   brakeEngaged
 Parameters
   none
 Returns
   none
 Description
   prints the muscle strength bar graph text message above the bar
 Notes
   message: ""MUSCLE SIGNAL STRENGTH"
 Author
   Ashton Coons, 5/20/25,
 ****************************************************************************/
  void printStrengthText();

/*******************************************************************************
 Function
   drawHorizontalEMGBar
 Parameters
   muscle sensor value
 Returns
   none
 Description
   maps the EMG signal (0-1023) to a horixontal bar graph (10-300) that displays
   during the runSystemFSM to the user screen for real-time feedback of the
   flex strength
 Notes

 Author
   Ashton Coons, 5/22/25,
 ****************************************************************************/
  void drawHorizontalEMGBar(int val);

/*******************************************************************************
 Function
   batteryLowPrompt
 Parameters
   battery voltage
 Returns
   none
 Description
   prints the warning during battery initialization if the battery level is below 
   50%. if the user wishes to continue, requires a manual press of the screen
 Notes
   message: "WARNING BATTERY LOW", "BATTERY LEVEL " volt "%"
            "Tap on screen if you wish to proceed"
 Author
   Ashton Coons, 5/23/25,
 ****************************************************************************/
  void batteryLowPrompt(float volt);

/*******************************************************************************
 Function
   batteryLowAccepted
 Parameters
   none
 Returns
   none
 Description
   changes the screen to black uopn a tap from the user confirming to proceed
   after low battery warning
 Notes
   message: "WARNING BATTERY LOW", "BATTERY LEVEL " volt "%"
            "Tap on screen if you wish to proceed"
 Author
   Ashton Coons, 5/20/25,
 ****************************************************************************/
  void batteryLowAccepted();

 #endif	/* PERIPHERAL_TEST_H */ // End of header guard