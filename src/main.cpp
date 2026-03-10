/* 
 * File:   main.cpp
 * Author: Ashton Coons
 * Brief: Main source file for MABPA System
 * Created on 2 19, 2025, 3:15pm
 * Modified on 2 19, 2025, 4:04pm
 */
#ifndef PERIPHERAL_TEST
 /******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
  #include <Arduino.h>

  #include "tftScreen.h"
  #include "startUpFSM.h"
  #include "systemRunningFSM.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

//enum for the system state machine (initialization and error)
  enum SystemState{
    INIIT, 
    READY, 
    ERROR
  };
  SystemState currentState = INIIT;
/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/

/*******************************************************************************
 * MAIN                                                                        *
 ******************************************************************************/
  void setup(){
    // //ensure 10 bit resolution
    analogReadResolution(10);
    //initialize the screen
    screenInit();
    //protect against statless init on cold boot
    currentState = INIIT;
  }

  void loop() {
    switch(currentState)
    {
      //initializes and checks the servo, muscle sensor, and battery to ensure
      //they are functioning as expected before using the system
      //if any fail, it will brick the system
      case INIIT:{
        StartupState startupResult = runStartupFSM();
        Serial.println(startupResult);
        if (startupResult == SYSTEM_READY)
        {
          //prints the muscle strength graph text to the screen
          printStrengthText();
          currentState = READY;
        }
        else if (startupResult == SYSTEM_ERROR)
        {
          currentState = ERROR;
        }
      break;} //INITIALIZING

      //if all the initialization checks are sucessful, run the braking FSM
      case READY:{
        RunState runResult = runSystemRunningFSM();
      break;}

      case ERROR:
        //requires power cycle reset in the case of battery fail or servo fail
        while(1);
      break;
    }//System State Machine Switch End
  }//loop end
    
#endif

