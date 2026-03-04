/* 
 * File:   startUpFSM.cpp
 * Author: Ashton Coons
 * Brief: FSM for the INITIALIZATION state in main
 * Created on 05 19, 2025, 3:15pm
 * Modified on 06 07, 2025, 4:04pm
 */
#ifndef PERIPHERAL_TEST
 /******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
  #include <Arduino.h>

  #include "startUpFSM.h"
  #include "muscleFSM.h"
  #include "battery.h"
  #include "tftScreen.h"
  #include "servoB.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/
 StartupState runStartupFSM(){
    static StartupState state = DRAW_LOGO;
    switch(state){
      //draws the team logo upon startup while hardware check initializes
      case DRAW_LOGO:
        drawLogo();
        state = INIT_BATTERY;
      break; //DRAW_LOGO

      //takes initial battery readings 
      case INIT_BATTERY:
        if (batteryInit())
        {
          //if battery initialization function is sucessful, update the battery readings
          updateBatteryReading();

          //if the battery level is below 50%, warm the user and require confirmation to continue
          if(getBatteryPercent(readBatteryVoltage()) < 50.0f)
          {
            //send battery low prompt to user and print percentage of battery
            batteryLowPrompt(getBatteryPercent(readBatteryVoltage()));
            state = LOW_BATT;
          }
          //if battery initialization checks are sucessful, initialize servo next
          else{
            state = INIT_SERVO;
          }
        }
        else{
          //if battery checks fail, print battery failure prompt and brick system
          batteryFailPrompt();
          state = SYSTEM_ERROR;
        }
      break; //INIT_BATTERY

      //low battery override confirmation
      case LOW_BATT:
        if(userReady()){
          batteryLowAccepted();
          state = INIT_SERVO;
        }
      break;

      //initializes the servo by checking to see if it is connected via a voltage dip upon actuation of the brake
      case INIT_SERVO:
        if(servoInit())
        {
          //if dip is detected, brake is connected and works 
          state = INIT_MUSCLE;
        }else{
          //if dip is not detected, write fail message and brick system
          servoFailPrompt();
          state = SYSTEM_ERROR;
        }
      break; //INIT_SERVO

      //initializes the muscle sensors and checks for proper placement
      //if sucessful, returns SYSTEM_READY to main
      case INIT_MUSCLE:{
        MuscleState result = runMuscleFSM();
        Serial.println(result);
        if (result == EMG_DONE){
          state = SYSTEM_READY;
        }
      break;} //INIT_MUSCLE

      case SYSTEM_ERROR:
      case SYSTEM_READY:
      break;
  }//end of switch for StartupState

  return state;
}

/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/

/*******************************************************************************
 * MAIN                                                                        *
 ******************************************************************************/
#endif